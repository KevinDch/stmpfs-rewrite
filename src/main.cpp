/** @file
 *
 * This file implements the mount thread for fuse_main and user-interface
 */

#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <iostream>
#include <fuse_ops.h>
#include <stmpfs_error.h>
#include <debug.h>

static struct fuse_operations fuse_ops =
        {
                .getattr    = do_getattr,
                .readlink   = do_readlink,
                .mknod      = do_mknod,
                .mkdir      = do_mkdir,
                .unlink     = do_unlink,
                .rmdir      = do_rmdir,
                .symlink    = do_symlink,
                .chmod      = do_chmod,
                .chown      = do_chown,
                .truncate   = do_truncate,
                .open       = do_open,
                .read       = do_read,
                .write      = do_write,
                .statfs     = do_statfs,
                .flush      = do_flush,
                .release    = do_release,
                .fsync      = do_fsync,
                .setxattr   = do_setxattr,
                .getxattr   = do_getxattr,
                .listxattr  = do_listxattr,
                .removexattr = do_removexattr,
                .opendir    = do_open,
                .readdir    = do_readdir,
                .releasedir = do_releasedir,
                .fsyncdir   = do_fsyncdir,
                .create     = do_create,
                .utimens    = do_utimens,
                .fallocate  = do_fallocate,
        };

static void usage(const char *progname)
{
    printf(
            "usage: %s mountpoint [options]\n"
            "\n"
            "general options:\n"
            "    -o opt,[opt...]        Mount options.\n"
            "    -h, --help             Print help.\n"
            "    -V, --version          Print version.\n"
            #ifdef CMAKE_BUILD_DEBUG
            "    -k, --hash_check       Enable hash check on every R/W.\n"
            #endif // CMAKE_BUILD_DEBUG
            "\n", progname);
}

enum {
    KEY_VERSION,
    KEY_HELP,
};

static struct fuse_opt fs_opts[] = {
        FUSE_OPT_KEY("-V",              KEY_VERSION),
        FUSE_OPT_KEY("--version",       KEY_VERSION),
        FUSE_OPT_KEY("-h",              KEY_HELP),
        FUSE_OPT_KEY("--help",          KEY_HELP),
        FUSE_OPT_END,
};

static int opt_proc(void *, const char *, int key, struct fuse_args *outargs)
{
    static struct fuse_operations ss_nullptr { };

    switch (key)
    {
        case KEY_VERSION:
            printf("%s Version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
            fuse_opt_add_arg(outargs, "--version");
            fuse_main(outargs->argc, outargs->argv, &ss_nullptr, nullptr);
            fuse_opt_free_args(outargs);
            exit(EXIT_SUCCESS);

        case KEY_HELP:
            usage(outargs->argv[0]);
            fuse_opt_add_arg(outargs, "-ho");
            fuse_main(outargs->argc, outargs->argv, &ss_nullptr, nullptr);
            fuse_opt_free_args(outargs);
            exit(EXIT_SUCCESS);

        default:
            return 1;
    }
}

int main(int argc, char ** argv)
{
    try
    {
        /// enable back tracing
        __path_to_executable = *argv;
        __check_addr2line();

        struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

        if (fuse_opt_parse(&args, nullptr, fs_opts, opt_proc) == -1)
        {
            throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
        }

        /// construct root
        auto cur_time = current_time();

        filesystem_root.fs_stat.st_mode = S_IFDIR | 0755;
        filesystem_root.fs_stat.st_atim = cur_time;
        filesystem_root.fs_stat.st_ctim = cur_time;
        filesystem_root.fs_stat.st_mtim = cur_time;

        /*
         * s: run single threaded
         * d: enable debugging
         * f: stay in foreground
         */
        fuse_opt_add_arg(&args, "-s");

#ifdef CMAKE_BUILD_DEBUG
        fuse_opt_add_arg(&args, "-d");
        fuse_opt_add_arg(&args, "-f");
#endif // CMAKE_BUILD_DEBUG

        int ret = fuse_main(args.argc, args.argv, &fuse_ops, nullptr);

        if (ret != 0)
        {
            throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
        }

        fuse_opt_free_args(&args);

        return EXIT_SUCCESS;
    }
    catch (std::exception & error)
    {
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }
}

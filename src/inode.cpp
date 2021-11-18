#include <inode.h>
#include <stmpfs_error.h>
#include <debug.h>
#include <cerrno>

void inode_t::snapshot(uint32_t version)
{
    auto it = children.find(version);
    if (it != children.end())
    {
        FUNCTION_INFO;
        OBTAIN_STACK_FRAME;

        throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
    }

    create_snapshot_for_children(version);
}

void inode_t::delete_snapshot(uint32_t version)
{
    auto it = children.find(version);
    if (it == children.end())
    {
        FUNCTION_INFO;
        OBTAIN_STACK_FRAME;

        throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
    }

    delete_snapshot_for_children(version);
}

inode_t *inode_t::get_inode_by_name(const std::string& _name, uint32_t version)
{
    auto it = children.find(version);
    if (it == children.end())
    {
        FUNCTION_INFO;
        OBTAIN_STACK_FRAME;

        throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
    }

    for (auto i : it->second)
    {
        if (i->name == _name)
        {
            return i;
        }
    }

    return nullptr;
}

void inode_t::create_snapshot_for_children(uint32_t version)
{
    if (children.empty())
    {
        return;
    }

    auto & it = children.at(0);
    children.emplace(version, it);
    snapshout_link_count += 1;

    for (auto &inode: it)
    {
        if (fs_stat.st_mode & S_IFDIR)
        {
            inode->snapshout_link_count += 1;
        }
        inode->create_snapshot_for_children(version);
    }
}

void inode_t::new_child(const std::string& _name)
{
    auto & it = children.at(0);
    for (auto & inode : it)
    {
        if (inode->name == _name)
        {
            FUNCTION_INFO;
            OBTAIN_STACK_FRAME;

            errno = EEXIST; // File exists (POSIX.1-2001).
            throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
        }
    }

    auto new_inode = new inode_t;
    new_inode->name = _name;

    it.push_back(new_inode);
}

inode_t::~inode_t()
{
    for (const auto& child_map : children)
    {
        for (auto inode : child_map.second)
        {
            if (inode->snapshout_link_count == 1)
            {
                delete inode;
            }
            else
            {
                inode->snapshout_link_count -= 1;
            }
        }
    }
}

void inode_t::delete_child(const std::string &_name)
{
    auto & it = children.at(0);
    for (auto inode = it.begin(); inode != it.end(); inode++)
    {
        if ((*inode)->name == _name)
        {
            if ((*inode)->snapshout_link_count == 1)
            {
                delete *inode;
            }
            else
            {
                (*inode)->snapshout_link_count -= 1;
            }

            it.erase(inode);

            return;
        }
    }

    FUNCTION_INFO;
    OBTAIN_STACK_FRAME;

    errno = ENOENT; // No such file or directory (POSIX.1-2001).
    throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
}

inode_t::inode_t() = default;

void inode_t::replicate_snapshot_0(const std::string &_name)
{
    auto & it = children.at(0);
    for (auto inode = it.begin(); inode != it.end(); inode++)
    {
        if (inode.operator*()->name == _name)
        {
            auto new_inode = new inode_t(**inode);
            new_inode->snapshout_link_count = 1;
            *inode = new_inode;
        }
    }
}

void inode_t::delete_snapshot_for_children(uint32_t version)
{
    if (children.empty())
    {
        return;
    }

    auto & it = children.at(version);

    for (auto &inode: it)
    {
        if (inode->snapshout_link_count == 1)
        {
            inode->delete_snapshot_for_children(version);
            delete inode;
        }
        else
        {
            inode->snapshout_link_count -= 1;
            inode->delete_snapshot_for_children(version);
        }
    }

    children.erase(version);
}

#pragma once

#include <gtkmm.h>

// ModelColumns class
class ModelColumns : public Glib::Object
{
public:
    // Create a new object
    static Glib::RefPtr<ModelColumns> create(Glib::ustring &branch1, Glib::ustring &version1)
    {
        return Glib::make_refptr_for_instance<ModelColumns>(new ModelColumns(branch1, version1));
    }
    static Glib::RefPtr<ModelColumns> create(const char *branch1, const char *version1)
    {
        Glib::ustring temp_branch(branch1), temp_version(version1);
        return Glib::make_refptr_for_instance<ModelColumns>(new ModelColumns(temp_branch, temp_version));
    }

    // Get Value of branch and version
    Glib::ustring get_branch_str()
    {
        return branch_prep.get_value();
    }

    Glib::ustring get_version_str()
    {
        return version_prep.get_value();
    }

    // Get PropertyProxy
    Glib::PropertyProxy<Glib::ustring> property_branch()
    {
        return branch_prep.get_proxy();
    }

    Glib::PropertyProxy<Glib::ustring> property_version()
    {
        return version_prep.get_proxy();
    }

private:
    // Use Glib::Property to bind
    Glib::Property<Glib::ustring> branch_prep, version_prep;

protected:
    // Register type and initalize properties
    ModelColumns(Glib::ustring branch, Glib::ustring version)
        : Glib::ObjectBase(typeid(ModelColumns)),
          Glib::Object(),
          branch_prep(*this, "branch", branch),
          version_prep(*this, "version", version)
    {
    }
};

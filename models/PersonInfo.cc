#include "PersonInfo.h"
#include "Department.h"
#include "Job.h"
#include <string>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

PersonInfo::PersonInfo(const Row &r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0)
    {
        if(!r["id"].isNull())
        {
            id_=std::make_shared<int32_t>(r["id"].as<int32_t>());
        }
        if(!r["job_id"].isNull())
        {
            jobId_=std::make_shared<int32_t>(r["job_id"].as<int32_t>());
        }
        if(!r["job_title"].isNull())
        {
            jobTitle_=std::make_shared<std::string>(r["job_title"].as<std::string>());
        }
        if(!r["department_id"].isNull())
        {
            departmentId_=std::make_shared<int32_t>(r["department_id"].as<int32_t>());
        }
        if(!r["department_name"].isNull())
        {
            departmentName_=std::make_shared<std::string>(r["department_name"].as<std::string>());
        }
        if(!r["manager_id"].isNull())
        {
            managerId_=std::make_shared<int32_t>(r["manager_id"].as<int32_t>());
        }
        if(!r["manager_full_name"].isNull())
        {
            managerFullName_=std::make_shared<std::string>(r["manager_full_name"].as<std::string>());
        }
        if(!r["first_name"].isNull())
        {
            firstName_=std::make_shared<std::string>(r["first_name"].as<std::string>());
        }
        if(!r["last_name"].isNull())
        {
            lastName_=std::make_shared<std::string>(r["last_name"].as<std::string>());
        }
        if(!r["hire_date"].isNull())
        {
            auto daysStr = r["hire_date"].as<std::string>();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            strptime(daysStr.c_str(),"%Y-%m-%d",&stm);
            time_t t = mktime(&stm);
            hireDate_=std::make_shared<::trantor::Date>(t*1000000);
        }
    }
    else
    {
        size_t offset = (size_t)indexOffset;
        if(offset + 7 > r.size())
        {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if(!r[index].isNull())
        {
            id_=std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 1;
        if(!r[index].isNull())
        {
            jobId_=std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 2;
        if(!r[index].isNull())
        {
            departmentId_=std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 3;
        if(!r[index].isNull())
        {
            managerId_=std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 4;
        if(!r[index].isNull())
        {
            firstName_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 5;
        if(!r[index].isNull())
        {
            lastName_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 6;
        if(!r[index].isNull())
        {
            auto daysStr = r[index].as<std::string>();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            strptime(daysStr.c_str(),"%Y-%m-%d",&stm);
            time_t t = mktime(&stm);
            hireDate_=std::make_shared<::trantor::Date>(t*1000000);
        }
        index = offset + 7;
        if(!r[index].isNull())
        {
            jobTitle_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 8;
        if(!r[index].isNull())
        {
            departmentName_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 9;
        if(!r[index].isNull())
        {
            managerFullName_=std::make_shared<std::string>(r[index].as<std::string>());
        }
    }

}

const int32_t &PersonInfo::getValueOfId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(id_)
        return *id_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &PersonInfo::getId() const noexcept
{
    return id_;
}

const int32_t &PersonInfo::getValueOfJobId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(jobId_)
        return *jobId_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &PersonInfo::getJobId() const noexcept
{
    return jobId_;
}

const std::string &PersonInfo::getValueOfJobTitle() const noexcept
{
    const static std::string defaultValue = std::string();
    if(jobTitle_)
        return *jobTitle_;
    return defaultValue;
}
const std::shared_ptr<std::string> &PersonInfo::getJobTitle() const noexcept
{
    return jobTitle_;
}

const int32_t &PersonInfo::getValueOfDepartmentId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(departmentId_)
        return *departmentId_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &PersonInfo::getDepartmentId() const noexcept
{
    return departmentId_;
}

const std::string &PersonInfo::getValueOfDepartmentName() const noexcept
{
    const static std::string defaultValue = std::string();
    if(departmentName_)
        return *departmentName_;
    return defaultValue;
}
const std::shared_ptr<std::string> &PersonInfo::getDepartmentName() const noexcept
{
    return departmentName_;
}

const int32_t &PersonInfo::getValueOfManagerId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(managerId_)
        return *managerId_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &PersonInfo::getManagerId() const noexcept
{
    return managerId_;
}

const std::string &PersonInfo::getValueOfManagerFullName() const noexcept
{
    const static std::string defaultValue = std::string();
    if(managerFullName_)
        return *managerFullName_;
    return defaultValue;
}
const std::shared_ptr<std::string> &PersonInfo::getManagerFullName() const noexcept
{
    return managerFullName_;
}

const std::string &PersonInfo::getValueOfFirstName() const noexcept
{
    const static std::string defaultValue = std::string();
    if(firstName_)
        return *firstName_;
    return defaultValue;
}
const std::shared_ptr<std::string> &PersonInfo::getFirstName() const noexcept
{
    return firstName_;
}

const std::string &PersonInfo::getValueOfLastName() const noexcept
{
    const static std::string defaultValue = std::string();
    if(lastName_)
        return *lastName_;
    return defaultValue;
}
const std::shared_ptr<std::string> &PersonInfo::getLastName() const noexcept
{
    return lastName_;
}

const ::trantor::Date &PersonInfo::getValueOfHireDate() const noexcept
{
    const static ::trantor::Date defaultValue = ::trantor::Date();
    if(hireDate_)
        return *hireDate_;
    return defaultValue;
}
const std::shared_ptr<::trantor::Date> &PersonInfo::getHireDate() const noexcept
{
    return hireDate_;
}

Json::Value PersonInfo::toJson() const
{
    Json::Value ret;
    if(getId())
    {
        ret["id"]=getValueOfId();
    }
    else
    {
        ret["id"]=Json::Value();
    }
    if(getJobId())
    {
        ret["job_id"]=getValueOfJobId();
    }
    else
    {
        ret["job_id"]=Json::Value();
    }
    if(getJobTitle())
    {
        ret["job_title"]=getValueOfJobTitle();
    }
    else
    {
        ret["job_title"]=Json::Value();
    }

    if(getDepartmentId())
    {
        ret["department_id"]=getValueOfDepartmentId();
    }
    else
    {
        ret["department_id"]=Json::Value();
    }
    if(getDepartmentName())
    {
        ret["department_name"]=getValueOfDepartmentName();
    }
    else
    {
        ret["manager_full_name"]=Json::Value();
    }

    if(getManagerId())
    {
        ret["manager_id"]=getValueOfManagerId();
    }
    else
    {
        ret["manager_id"]=Json::Value();
    }

    if(getManagerFullName())
    {
        ret["manager_full_name"]=getValueOfManagerFullName();
    }
    else
    {
        ret["manager_full_name"]=Json::Value();
    }

    if(getFirstName())
    {
        ret["first_name"]=getValueOfFirstName();
    }
    else
    {
        ret["first_name"]=Json::Value();
    }
    if(getLastName())
    {
        ret["last_name"]=getValueOfLastName();
    }
    else
    {
        ret["last_name"]=Json::Value();
    }
    if(getHireDate())
    {
        ret["hire_date"]=getHireDate()->toDbStringLocal();
    }
    else
    {
        ret["hire_date"]=Json::Value();
    }
    if(getId())
    {
        ret["id"]=getValueOfId();
    }
    else
    {
        ret["id"]=Json::Value();
    }
    return ret;
}

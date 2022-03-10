#pragma once
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/SqlBinder.h>
#include <drogon/orm/Mapper.h>
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <json/json.h>
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <stdint.h>
#include <iostream>

namespace drogon
{
namespace orm
{
class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;
}
}
namespace drogon_model
{
namespace org_chart
{

class PersonInfo
{
  public:

    explicit PersonInfo(const drogon::orm::Row &r, const ssize_t indexOffset = 0) noexcept;

    PersonInfo() = default;

    /**  For column id  */
    ///Get the value of the column id, returns the default value if the column is null
    const int32_t &getValueOfId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getId() const noexcept;

    /**  For column job_id  */
    ///Get the value of the column job_id, returns the default value if the column is null
    const int32_t &getValueOfJobId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getJobId() const noexcept;

    /**  For column job_title  */
    ///Get the value of the column job_title, returns the default value if the column is null
    const std::string &getValueOfJobTitle() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getJobTitle() const noexcept;

    /**  For column department_id  */
    ///Get the value of the column department_id, returns the default value if the column is null
    const int32_t &getValueOfDepartmentId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getDepartmentId() const noexcept;

    /**  For column department_name  */
    ///Get the value of the column department_name, returns the default value if the column is null
    const std::string &getValueOfDepartmentName() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getDepartmentName() const noexcept;

    /**  For column manager_id  */
    ///Get the value of the column manager_id, returns the default value if the column is null
    const int32_t &getValueOfManagerId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getManagerId() const noexcept;

    /**  For column manager_full_name  */
    ///Get the value of the column first_name, returns the default value if the column is null
    const std::string &getValueOfManagerFullName() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getManagerFullName() const noexcept;

    /**  For column first_name  */
    ///Get the value of the column first_name, returns the default value if the column is null
    const std::string &getValueOfFirstName() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getFirstName() const noexcept;

    /**  For column last_name  */
    ///Get the value of the column last_name, returns the default value if the column is null
    const std::string &getValueOfLastName() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getLastName() const noexcept;

    /**  For column hire_date  */
    ///Get the value of the column hire_date, returns the default value if the column is null
    const ::trantor::Date &getValueOfHireDate() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getHireDate() const noexcept;

    Json::Value toJson() const;
  private:
    friend drogon::orm::Mapper<PersonInfo>;
    std::shared_ptr<int32_t> id_;
    std::shared_ptr<int32_t> jobId_;
    std::shared_ptr<std::string> jobTitle_;
    std::shared_ptr<int32_t> departmentId_;
    std::shared_ptr<std::string> departmentName_;
    std::shared_ptr<int32_t> managerId_;
    std::shared_ptr<std::string> managerFullName_;
    std::shared_ptr<std::string> firstName_;
    std::shared_ptr<std::string> lastName_;
    std::shared_ptr<::trantor::Date> hireDate_;
};
} // namespace org_chart
} // namespace drogon_model

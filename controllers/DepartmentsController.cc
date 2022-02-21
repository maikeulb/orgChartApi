#include "DepartmentsController.h"
#include "../models/Department.h"
#include "../models/Person.h"

using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace drogon {
    template<>
    inline Department fromRequest(const HttpRequest &req) {
        auto jsonPtr = req.getJsonObject();
        auto jsonVal = (*jsonPtr);
        auto department = Department(jsonVal);
        return department;
    }
}

void DepartmentsController::getAllDepartments(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int offset, int limit) const
{
    LOG_DEBUG << "getAllDepartments" << " offset, " << offset << " limit, " << limit;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    auto departments = mp.orderBy(Department::Cols::_id).limit(limit).offset(offset).findAll();

    Json::Value ret;
    for (auto d : departments) {
        ret.append(d.toJson());
    }

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void DepartmentsController::getDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int departmentId) const
{
    LOG_DEBUG << "getDepartment departmentId: "<< departmentId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    auto dep = mp.findOne(Criteria(Department::Cols::_id, CompareOperator::EQ, departmentId));

    Json::Value ret;
    ret = dep.toJson(); 

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void DepartmentsController::newDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Department &&pNewDepartment) const
{
    LOG_DEBUG << "newDepartment";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    mp.insert(pNewDepartment);

    Json::Value ret;
    ret["result"] = pNewDepartment.toJson();
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void DepartmentsController::updateDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId, Department &&pDepartmentDetails) const
{
    LOG_DEBUG << "updateDepartment personId: " << personId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    auto dep = mp.findOne(Criteria(Department::Cols::_id, CompareOperator::EQ, personId));

    if (pDepartmentDetails.getName() != nullptr) {
        dep.setName(pDepartmentDetails.getValueOfName());
    }

    mp.update(dep);

    Json::Value ret;
    ret["result"] = dep.toJson();

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void DepartmentsController::deleteDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int personId) const
{
    LOG_DEBUG << "deleteDepartment, personId: ";

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    mp.deleteBy(Criteria(Department::Cols::_id, CompareOperator::EQ, personId));

    Json::Value ret;
    ret["result"] = "OK";

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void DepartmentsController::getDepartmentPersons(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int departmentId) const
{
    LOG_DEBUG << "getDepartmentPersons departmentId: "<< departmentId;

    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    auto dep = mp.findOne(Criteria(Department::Cols::_id, CompareOperator::EQ, departmentId));

    dep.getPersons(dbClientPtr, 
      [callback](const std::vector<Person> persons) {
          Json::Value ret;
          for (auto p : persons) {
              ret.append(p.toJson());
          }
          auto resp=HttpResponse::newHttpJsonResponse(ret);
          callback(resp);
      },
      [](const DrogonDbException &e) {
          LOG_ERROR << "error:" << e.base().what();
      }
    );
}

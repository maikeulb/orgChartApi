#include "DepartmentsController.h"
#include "../models/Department.h"
#include "../models/Person.h"

using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace drogon {
    template<>
    inline Department fromRequest(const HttpRequest &req) {
        auto jsonPtr = req.getJsonObject();
        auto json = *jsonPtr;
        auto department = Department(json);
        return department;
    }
}

void DepartmentsController::getAllDepartments(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int offset, int limit) const
{
    LOG_DEBUG << "getAllDepartments" << " offset: " << offset << " limit: " << limit;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Department> mp(dbClientPtr);
        auto departments = mp.orderBy(Department::Cols::_id).offset(offset).limit(limit).findFutureAll().get();

        Json::Value ret;
        for (auto d : departments) {
            ret.append(d.toJson());
        }
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void DepartmentsController::getDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int departmentId) const
{
    LOG_DEBUG << "getDepartment departmentId: "<< departmentId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Department> mp(dbClientPtr);
        auto department = mp.findFutureByPrimaryKey(departmentId).get();

        Json::Value ret = department.toJson(); 
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void DepartmentsController::newDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Department &&pDepartment) const
{
    LOG_DEBUG << "newDepartment";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Department> mp(dbClientPtr);
        auto department = mp.insertFuture(pDepartment).get();

        Json::Value ret = department.toJson();
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k201Created);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void DepartmentsController::updateDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int departmentId, Department &&pDepartmentDetails) const
{
    LOG_DEBUG << "updateDepartment departmentId: " << departmentId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Department> mp(dbClientPtr);
        auto department = mp.findFutureByPrimaryKey(departmentId).get();

        if (pDepartmentDetails.getName() != nullptr) {
            department.setName(pDepartmentDetails.getValueOfName());
        }

        mp.update(department);

        Json::Value ret = department.toJson();
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k204NoContent);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void DepartmentsController::deleteDepartment(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int departmentId) const
{
    LOG_DEBUG << "deleteDepartment departmentId: ";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Department> mp(dbClientPtr);
        mp.deleteFutureBy(Criteria(Person::Cols::_id, CompareOperator::EQ, departmentId)).get();

        auto resp=HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k204NoContent);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void DepartmentsController::getDepartmentPersons(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int departmentId) const
{
    LOG_DEBUG << "getDepartmentPersons departmentId: "<< departmentId;
    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    auto department = mp.findFutureByPrimaryKey(departmentId).get();

    department.getPersons(dbClientPtr, 
      [callback](const std::vector<Person> persons) {
          if (persons.empty()) {
              auto resp = HttpResponse::newHttpResponse();
              resp->setStatusCode(HttpStatusCode::k404NotFound);
              callback(resp);
          } else {
              Json::Value ret;
              for (auto p : persons) {
                  ret.append(p.toJson());
              }
              auto resp=HttpResponse::newHttpJsonResponse(ret);
              resp->setStatusCode(HttpStatusCode::k200OK);
              callback(resp);
          }
      },
      [callback](const DrogonDbException &e) {
          LOG_ERROR << e.base().what();
          auto resp = HttpResponse::newHttpResponse();
          resp->setStatusCode(HttpStatusCode::k500InternalServerError);
          callback(resp);
      }
    );
}

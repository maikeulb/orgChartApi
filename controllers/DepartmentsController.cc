#include "DepartmentsController.h"
#include <string>
#include <memory>
#include <utility>
#include <vector>
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
}  // namespace drogon

void DepartmentsController::get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const {
    LOG_DEBUG << "get";
    auto offset = req->getOptionalParameter<int>("offset").value_or(0);
    auto limit = req->getOptionalParameter<int>("limit").value_or(25);
    auto sortField = req->getOptionalParameter<std::string>("sort_field").value_or("id");
    auto sortOrder = req->getOptionalParameter<std::string>("sort_order").value_or("asc");
    auto sortOrderEnum = sortOrder == "asc" ? SortOrder::ASC : SortOrder::DESC;

    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Department> mp(dbClientPtr);
        auto departments = mp.orderBy(sortField, sortOrderEnum).offset(offset).limit(limit).findFutureAll().get();

        Json::Value ret;
        for (auto d : departments) {
            ret.append(d.toJson());
        }
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret{};
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void DepartmentsController::getOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int departmentId) const {
    LOG_DEBUG << "getOne departmentId: "<< departmentId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Department> mp(dbClientPtr);
        Department department;
        try {
            department = mp.findFutureByPrimaryKey(departmentId).get();
        } catch (const DrogonDbException & e) {
            Json::Value ret{};
            ret["error"] = "resource not found";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k404NotFound);
            callback(resp);
        }

        Json::Value ret{};
        ret = department.toJson();
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret;
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void DepartmentsController::createOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, Department &&pDepartment) const {
    LOG_DEBUG << "createOne";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Department> mp(dbClientPtr);
        auto department = mp.insertFuture(pDepartment).get();

        Json::Value ret{};
        ret = department.toJson();
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k201Created);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret;
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void DepartmentsController::updateOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int departmentId, Department &&pDepartmentDetails) const {
    LOG_DEBUG << "updateOne departmentId: " << departmentId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Department> mp(dbClientPtr);
        Department department;
        try {
            department = mp.findFutureByPrimaryKey(departmentId).get();
        } catch (const DrogonDbException & e) {
            Json::Value ret{};
            ret["error"] = "resource not found";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k404NotFound);
            callback(resp);
        }

        if (pDepartmentDetails.getName() != nullptr) {
            department.setName(pDepartmentDetails.getValueOfName());
        }

        mp.update(department);

        Json::Value ret{};
        ret = department.toJson();
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k204NoContent);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret{};
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void DepartmentsController::deleteOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int departmentId) const {
    LOG_DEBUG << "deleteOne departmentId: ";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Department> mp(dbClientPtr);
        mp.deleteFutureBy(Criteria(Person::Cols::_id, CompareOperator::EQ, departmentId)).get();

        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k204NoContent);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret{};
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void DepartmentsController::getDepartmentPersons(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int departmentId) const {
    LOG_DEBUG << "getDepartmentPersons departmentId: "<< departmentId;
    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    Department department;
    try {
        department = mp.findFutureByPrimaryKey(departmentId).get();
    } catch (const DrogonDbException & e) {
        Json::Value ret{};
        ret["error"] = "resource not found";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k404NotFound);
        callback(resp);
    }

    department.getPersons(dbClientPtr,
      [callbackPtr](const std::vector<Person> persons) {
          if (persons.empty()) {
              Json::Value ret{};
              ret["error"] = "resource not found";
              auto resp = HttpResponse::newHttpJsonResponse(ret);
              resp->setStatusCode(HttpStatusCode::k404NotFound);
              (*callbackPtr)(resp);
          } else {
              Json::Value ret{};
              for (auto p : persons) {
                  ret.append(p.toJson());
              }
              auto resp = HttpResponse::newHttpJsonResponse(ret);
              resp->setStatusCode(HttpStatusCode::k200OK);
              (*callbackPtr)(resp);
          }
      },
      [callbackPtr](const DrogonDbException &e) {
          LOG_ERROR << e.base().what();
          Json::Value ret{};
          ret["error"] = "database error";
          auto resp = HttpResponse::newHttpJsonResponse(ret);
          resp->setStatusCode(HttpStatusCode::k500InternalServerError);
          (*callbackPtr)(resp);
      });
}

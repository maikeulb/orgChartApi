#include "DepartmentsController.h"
#include "../utils/utils.h"
#include "../models/Person.h"
#include <string>
#include <memory>
#include <utility>
#include <vector>

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

    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient();
    Mapper<Department> mp(dbClientPtr);
    mp.orderBy(sortField, sortOrderEnum).offset(offset).limit(limit).findAll(
        [callbackPtr](const std::vector<Department> &departments) {
            Json::Value ret{};
            for (auto d : departments) {
                ret.append(d.toJson());
            }
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k200OK);
            (*callbackPtr)(resp);
        },
        [callbackPtr](const DrogonDbException &e) {
            LOG_ERROR << e.base().what();
            auto resp = HttpResponse::newHttpJsonResponse(makeErrResp("database error"));
            resp->setStatusCode(HttpStatusCode::k500InternalServerError);
            (*callbackPtr)(resp);
    });
}

void DepartmentsController::getOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int departmentId) const {
    LOG_DEBUG << "getOne departmentId: "<< departmentId;
    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    mp.findByPrimaryKey(
        departmentId,
        [callbackPtr](const Department &department) {
            Json::Value ret{};
            ret = department.toJson();
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k201Created);
            (*callbackPtr)(resp);
        },
        [callbackPtr](const DrogonDbException &e) {
            const drogon::orm::UnexpectedRows *s = dynamic_cast<const drogon::orm::UnexpectedRows *>(&e.base());
            if(s) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k404NotFound);
                (*callbackPtr)(resp);
                return;
            }
            LOG_ERROR << e.base().what();
            auto resp = HttpResponse::newHttpJsonResponse(makeErrResp("database error"));
            resp->setStatusCode(HttpStatusCode::k500InternalServerError);
            (*callbackPtr)(resp);
    });
}

void DepartmentsController::createOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, Department &&pDepartment) const {
    LOG_DEBUG << "createOne";
    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    mp.insert(
        pDepartment,
        [callbackPtr](const Department &department) {
            Json::Value ret{};
            ret = department.toJson();
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k201Created);
            (*callbackPtr)(resp);
        },
        [callbackPtr](const DrogonDbException &e) {
            LOG_ERROR << e.base().what();
            auto resp = HttpResponse::newHttpJsonResponse(makeErrResp("database error"));
            resp->setStatusCode(HttpStatusCode::k500InternalServerError);
            (*callbackPtr)(resp);
    });
}

void DepartmentsController::updateOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int departmentId, Department &&pDepartmentDetails) const {
    LOG_DEBUG << "updateOne departmentId: " << departmentId;
    auto dbClientPtr = drogon::app().getDbClient();

    // blocking IO
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

    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    mp.update(
        department,
        [callbackPtr](const std::size_t count)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(HttpStatusCode::k204NoContent);
            (*callbackPtr)(resp);
        },
        [callbackPtr](const DrogonDbException &e)
        {
            LOG_ERROR << e.base().what();
            auto resp = HttpResponse::newHttpJsonResponse(makeErrResp("database error"));
            resp->setStatusCode(HttpStatusCode::k500InternalServerError);
            (*callbackPtr)(resp);
        }
    );
}

void DepartmentsController::deleteOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int departmentId) const {
    LOG_DEBUG << "deleteOne departmentId: ";
    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Department> mp(dbClientPtr);
    mp.deleteBy(
        Criteria(Department::Cols::_id, CompareOperator::EQ, departmentId),
        [callbackPtr](const std::size_t count) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(HttpStatusCode::k204NoContent);
            (*callbackPtr)(resp);
        },
        [callbackPtr](const DrogonDbException &e) {
            LOG_ERROR << e.base().what();
            auto resp = HttpResponse::newHttpJsonResponse(makeErrResp("database error"));
            resp->setStatusCode(HttpStatusCode::k500InternalServerError);
            (*callbackPtr)(resp);
    });
}

void DepartmentsController::getDepartmentPersons(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int departmentId) const {
    LOG_DEBUG << "getDepartmentPersons departmentId: "<< departmentId;
    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient();

    // blocking IO
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

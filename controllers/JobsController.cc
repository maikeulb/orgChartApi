#include "JobsController.h"
#include "../models/Person.h"

using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace drogon {
    template<>
    inline Job fromRequest(const HttpRequest &req) {
        auto json = req.getJsonObject();
        auto job = Job(*json);
        return job;
    }
}

void JobsController::get(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG << "get";
    auto offset = req->getOptionalParameter<int>("offset").value_or(0);
    auto limit = req->getOptionalParameter<int>("limit").value_or(25);
    auto sortField = req->getOptionalParameter<std::string>("sort_field").value_or("id");
    auto sortOrder = req->getOptionalParameter<std::string>("sort_order").value_or("asc");
    auto sortOrderEnum = sortOrder == "asc" ? SortOrder::ASC : SortOrder::DESC;

    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Job> mp(dbClientPtr);
        auto jobs = mp.orderBy(sortField, sortOrderEnum).offset(offset).limit(limit).findFutureAll().get();

        Json::Value ret;
        for (auto j : jobs) {
            ret.append(j.toJson());
        }
        auto resp=HttpResponse::newHttpJsonResponse(ret);
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

void JobsController::getOne(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId) const
{
    LOG_DEBUG << "getOne jobId: "<< jobId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Job> mp(dbClientPtr);
        Job job;
        try {
            job = mp.findFutureByPrimaryKey(jobId).get();
        } catch (const DrogonDbException & e) {
            Json::Value ret;
            ret["error"] = "resource not found";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k404NotFound);
            callback(resp);
        }

        Json::Value ret = job.toJson();
        auto resp=HttpResponse::newHttpJsonResponse(ret);
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

void JobsController::createOne(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Job &&pJob) const
{
    LOG_DEBUG << "createOne";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Job> mp(dbClientPtr);
        auto job = mp.insertFuture(pJob).get();

        Json::Value ret = job.toJson();
        auto resp=HttpResponse::newHttpJsonResponse(ret);
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

void JobsController::updateOne(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId, Job &&pJobDetails) const
{
    LOG_DEBUG << "updateOne jobId: " << jobId;
    auto jsonPtr = req->jsonObject();
    if (!jsonPtr) {
      Json::Value ret;
      ret["error"]="No json object is found in the request";
      auto resp = HttpResponse::newHttpResponse();
      resp->setStatusCode(HttpStatusCode::k400BadRequest);
      callback(resp);
      return;
    }
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Job> mp(dbClientPtr);
        Job job;
        try {
            job = mp.findFutureByPrimaryKey(jobId).get();
        } catch (const DrogonDbException & e) {
            Json::Value ret;
            ret["error"] = "resource not found";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k404NotFound);
            callback(resp);
        }

        if (pJobDetails.getTitle() != nullptr) {
            job.setTitle(pJobDetails.getValueOfTitle());
        }
        mp.update(job);

        Json::Value ret = job.toJson();
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k204NoContent);
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

void JobsController::deleteOne(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId) const
{
    LOG_DEBUG << "deleteOne jobId: ";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Job> mp(dbClientPtr);
        mp.deleteFutureBy(Criteria(Person::Cols::_id, CompareOperator::EQ, jobId)).get();

        auto resp=HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k204NoContent);
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

void JobsController::getJobPersons(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId) const
{
    LOG_DEBUG << "getJobPersons jobId: "<< jobId;
    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Job> mp(dbClientPtr);
    Job job;
    try {
        job = mp.findFutureByPrimaryKey(jobId).get();
    } catch (const DrogonDbException & e) {
        Json::Value ret;
        ret["error"] = "resource not found";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k404NotFound);
        callback(resp);
    }

    job.getPersons(dbClientPtr,
        [callbackPtr](const std::vector<Person> persons) {
           if (persons.empty()) {
              Json::Value ret;
              ret["error"] = "resource not found";
              auto resp = HttpResponse::newHttpJsonResponse(ret);
              resp->setStatusCode(HttpStatusCode::k404NotFound);
              (*callbackPtr)(resp);
          } else {
              Json::Value ret;
              for (auto p : persons) {
                  ret.append(p.toJson());
              }
              auto resp=HttpResponse::newHttpJsonResponse(ret);
              resp->setStatusCode(HttpStatusCode::k200OK);
              (*callbackPtr)(resp);
          }
        },
        [callbackPtr](const DrogonDbException &e) {
          LOG_ERROR << e.base().what();
          Json::Value ret;
          ret["error"] = "database error";
          auto resp = HttpResponse::newHttpJsonResponse(ret);
          resp->setStatusCode(HttpStatusCode::k500InternalServerError);
          (*callbackPtr)(resp);
        }
    );
}

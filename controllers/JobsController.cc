#include "JobsController.h"
#include "../models/Job.h"
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

void JobsController::getAllJobs(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int offset, int limit) const
{
    LOG_DEBUG << "getAllJobs" << " offset: " << offset << " limit: " << limit;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Job> mp(dbClientPtr);
        auto jobs = mp.orderBy(Job::Cols::_id).offset(offset).limit(limit).findFutureAll().get();

        Json::Value ret;
        for (auto j : jobs) {
            ret.append(j.toJson());
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

void JobsController::getJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId) const
{
    LOG_DEBUG << "getJob jobId: "<< jobId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Job> mp(dbClientPtr);
        auto job = mp.findFutureByPrimaryKey(jobId).get();

        Json::Value ret = job.toJson(); 
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

void JobsController::newJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, Job &&pJob) const
{
    LOG_DEBUG << "newJob";
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
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void JobsController::updateJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId, Job &&pJobDetails) const
{
    LOG_DEBUG << "updateJob jobId: " << jobId;
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Job> mp(dbClientPtr);
        auto job = mp.findFutureByPrimaryKey(jobId).get();

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
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

void JobsController::deleteJob(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId) const
{
    LOG_DEBUG << "deleteJob jobId: ";
    try {
        auto dbClientPtr = drogon::app().getDbClient();

        Mapper<Job> mp(dbClientPtr);
        mp.deleteFutureBy(Criteria(Person::Cols::_id, CompareOperator::EQ, jobId)).get();

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

void JobsController::getJobPersons(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, int jobId) const
{
    LOG_DEBUG << "getJobPersons jobId: "<< jobId;
    auto dbClientPtr = drogon::app().getDbClient();

    Mapper<Job> mp(dbClientPtr);
    auto job = mp.findFutureByPrimaryKey(jobId).get();

    job.getPersons(dbClientPtr, 
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

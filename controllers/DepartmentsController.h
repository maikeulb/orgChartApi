#pragma once

#include <drogon/HttpController.h>
#include "../models/Department.h"

using namespace drogon;
using namespace drogon_model::org_chart;

class DepartmentsController : public drogon::HttpController<DepartmentsController> {
 public:
    METHOD_LIST_BEGIN
      ADD_METHOD_TO(DepartmentsController::get, "/departments", Get);
      ADD_METHOD_TO(DepartmentsController::getOne, "/departments/{1}", Get);
      ADD_METHOD_TO(DepartmentsController::createOne, "/departments", Post, "LoginFilter");
      ADD_METHOD_TO(DepartmentsController::updateOne, "/departments/{1}", Put, "LoginFilter");
      ADD_METHOD_TO(DepartmentsController::deleteOne, "/departments/{1}", Delete, "LoginFilter");
      ADD_METHOD_TO(DepartmentsController::getDepartmentPersons, "/departments/{1}/persons", Get, "LoginFilter");
    METHOD_LIST_END

    void get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    void getOne(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr &)> &&callback, int pDepartmentId) const;
    void createOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, Department &&pDepartment) const;
    void updateOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int pDepartmentId, Department &&pDepartment) const;
    void deleteOne(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int pDepartmentId) const;
    void getDepartmentPersons(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int departmentId) const;
};

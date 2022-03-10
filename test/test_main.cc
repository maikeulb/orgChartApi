#define DROGON_TEST_MAIN
#include <drogon/drogon_test.h>
#include <drogon/drogon.h>

// DROGON_TEST(RemoteAPITest)
// {
//     auto client = drogon::HttpClient::newHttpClient("http://localhost:8848");
//     auto req = drogon::HttpRequest::newHttpRequest();
//     req->setPath("/");
//     client->sendRequest(req, [TEST_CTX](drogon::ReqResult res, const drogon::HttpResponsePtr& resp) {
//         // There's nothing we can do if the request didn't reach the server
//         // or the server generated garbage.
//         REQUIRE(res == drogon::ReqResult::Ok);
//         REQUIRE(resp != nullptr);

//         CHECK(resp->getStatusCode() == drogon::k200OK);
//         CHECK(resp->contentType() == drogon::CT_APPLICATION_JSON);
//     });
// }

// DROGON_TEST(BasicTest)
// {
//     // Add your tests here
// }

int main(int argc, char** argv)
{
    using namespace drogon;

    std::promise<void> p1;
    std::future<void> f1 = p1.get_future();

    // Start the main loop on another thread
    std::thread thr([&]() {
        // Queues the promise to be fulfilled after starting the loop
        app().getLoop()->queueInLoop([&p1]() { p1.set_value(); });
        app().run();
    });

    // The future is only satisfied after the event loop started
    f1.get();
    int status = test::run(argc, argv);

    // Ask the event loop to shutdown and wait
    app().getLoop()->queueInLoop([]() { app().quit(); });
    thr.join();
    return status;
}

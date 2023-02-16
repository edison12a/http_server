#include <iostream>
#include <string>
#include <vector>
#include <cstdarg>
#include <sstream>
#include <any>
#include <regex>
#include <time.h>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <csignal>

#include <openssl/sha.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/MySQL/MySQL.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MetaColumn.h>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPCookie.h>
#include <Poco/Net/FilePartSource.h>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/SecureSMTPClientSession.h>
#include <Poco/Net/StringPartSource.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/KeyConsoleHandler.h>
#include <Poco/Net/ConsoleCertificateHandler.h>

#include <Poco/StreamCopier.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/JSON/Parser.h>
#include <Poco/URI.h>
#include <Poco/SharedPtr.h>
#include <Poco/Exception.h>
#include <Poco/Path.h>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/MapConfiguration.h>
#include <Poco/Util/LayeredConfiguration.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>

#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>

#include <podofo/podofo.h>
#include <sw/redis++/redis++.h>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

class MyRequestHandler : public HTTPRequestHandler
{
public:
  virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
  {
    resp.setStatus(HTTPResponse::HTTP_OK);
    resp.setContentType("text/html");

    ostream &out = resp.send();
    out << "<h1>Hello world!</h1>"
        << "<p>Count: " << ++count << "</p>"
        << "<p>Host: " << req.getHost() << "</p>"
        << "<p>Method: " << req.getMethod() << "</p>"
        << "<p>URI: " << req.getURI() << "</p>";
    out.flush();

    cout << endl
         << "Response sent for count=" << count
         << " and URI=" << req.getURI() << endl;
  }

private:
  static int count;
};

int MyRequestHandler::count = 0;

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
  virtual HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &)
  {
    return new MyRequestHandler;
  }
};

class MyServerApp : public ServerApplication
{
protected:
  int main(const vector<string> &)
  {
    HTTPServer s(new MyRequestHandlerFactory, ServerSocket(9090), new HTTPServerParams);

    s.start();
    cout << endl
         << "Server started" << endl;

    waitForTerminationRequest(); // wait for CTRL-C or kill

    cout << endl
         << "Shutting down..." << endl;
    s.stop();

    return Application::EXIT_OK;
  }
};

int main(int argc, char **argv)
{
  MyServerApp app;
  return app.run(argc, argv);
}
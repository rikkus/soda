#include "SODAParser.h"
#include <qdom.h>
#include <qdir.h>
#include <qfile.h>
#include <qcstring.h>
#include <dcopclient.h>

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    qDebug("Usage: %s filename.xml", argv[0]);
    return 1;
  }

  QDomDocument doc;

  QFile soapFile(argv[1]);

  if (!soapFile.open(IO_ReadOnly))
  {
    qDebug("Can't read file %s", argv[1]);
    return 1;
  }

  doc.setContent(&soapFile);

  if (doc.isNull())
  {
    qDebug("Document is fucked");
    return 1;
  }

  QDomElement envelope = doc.documentElement();

  if (envelope.isNull())
  {
    qDebug("Document element is fucked");
    return false;
  }

  QFile xmlrpcKeyFile(QDir::homeDirPath() + "/.kxmlrpcd");

  if (xmlrpcKeyFile.size() > 256)
  {
    qDebug("Hmm, XMLRPC key file is quite large. Too scary for me.");
    return 1;
  }

  if (!xmlrpcKeyFile.open(IO_ReadOnly))
  {
    qDebug("Can't open XMLRPC key file");
    return 1;
  }

  QString xmlrpcKey(xmlrpcKeyFile.readAll());

  xmlrpcKeyFile.close();

  SODA::Parser parser(envelope, xmlrpcKey);

  QCString    application;
  QCString    object;
  QCString    method;
  QByteArray  marshalledParameters;

  if (!parser.getCall(application, object, method, marshalledParameters))
  {
    qDebug("Parsing failed");

    QString faultCode;

    switch (parser.faultCode())
    {
      case SODA::Parser::NoFault:
        faultCode = "No fault";
        break;

      case SODA::Parser::VersionMismatch:
        faultCode = "Version mismatch";
        break;

      case SODA::Parser::MustUnderstand:
        faultCode = "Must understand";
        break;

      case SODA::Parser::Client:
        faultCode = "Client";
        break;

      case SODA::Parser::Server:
        faultCode = "Server";
        break;
    }

    qDebug("Fault code    : %s", faultCode.latin1());
    qDebug("Fault string  : %s", parser.faultString().latin1());
    qDebug("Fault detail  : %s", parser.faultDetail().latin1());
    return 1;
  }

  qDebug("dcop:/%s/%s/%s", application.data(), object.data(), method.data());

  DCOPClient dcopClient;

  if (!dcopClient.attach())
  {
    qDebug("Couldn't attach to DCOP");
    return 1;
  }

  QCString replyType;
  QByteArray replyData;

  bool ok =
    dcopClient.call
    (application, object, method, marshalledParameters, replyType, replyData);

  if (!ok)
  {
    qDebug("DCOP call failed");
    return 1;
  }

  qDebug("DCOP call succeded");
}


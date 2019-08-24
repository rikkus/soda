/*
    SODA::Parser - Part of SODA, a SOAP<->DCOP bridge.

    Copyright 2001 Rik Hemsley (rikkus) <rik@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "SODAParser.h"
#include <qdom.h>

namespace SODA
{
  static const char * soapEnvelopeAttribute =
    "http://schemas.xmlsoap.org/soap/envelope/";

  static const char * soapEncodingAttribute =
    "http://schemas.xmlsoap.org/soap/encoding/";

  static const char * dcopAttribute =
    "http://developer.kde.org/dcop/";

  class Parser::Private
  {
    public:

      Private(const QString & authKey)
        : haveHeader  (false),
          haveBody    (false),
          authorized  (false),
          authKey     (authKey),
          faultCode   (Parser::NoFault)
      {
        // Empty.
      }

      bool          haveHeader;
      bool          haveBody;
      bool          authorized;
      QString       authKey;
      FaultCode     faultCode;
      QString       faultString;
      QString       faultDetail;

      QString       nameSpace;

      QCString      application;
      QCString      object;
      QCString      method;
      QCStringList  paramTypeList;
      QByteArray    data;
  };

  Parser::Parser(const QDomElement & e, const QString & authKey)
  {
    d = new Private(authKey);

    parseEnvelope(e);
  }

  Parser::~Parser()
  {
    delete d;
  }

    Parser::FaultCode
  Parser::faultCode() const
  {
    return d->faultCode;
  }

    QString
  Parser::faultString() const
  {
    return d->faultString;
  }

    QString
  Parser::faultDetail() const
  {
    return d->faultDetail;
  }

    bool
  Parser::getCall
  (
   QCString & app,
   QCString & object,
   QCString & method,
   QByteArray & data
  )
  {
    if (NoFault != d->faultCode)
      return false;

    app     = d->application;
    object  = d->object;

    method  = d->method + '(';

    QCStringList::ConstIterator it;

    for (it = d->paramTypeList.begin(); it != d->paramTypeList.end(); ++it)
    {
      method += *it;

      if (it != d->paramTypeList.fromLast())
        method += ", ";
    }

    method += ')';

    data = d->data;

    return true;
  }

    bool
  Parser::parseEnvelope(const QDomElement & envelope)
  {
    QString e = elementName(envelope.tagName());

    if ("Envelope" != elementName(envelope.tagName()))
    {
      d->faultCode = Client;
      d->faultString = "This isn't a SOAP envelope";
      return false;
    }

    d->nameSpace = elementNamespace(envelope.tagName());

    if (!d->nameSpace)
    {
      d->faultCode = Client;
      d->faultString = "Envelope does not have a nameSpace";
      return false;
    }

    QString nameSpaceID = envelope.attribute("xmlns:" + d->nameSpace);

    if (!!nameSpaceID)
    {
      if (soapEnvelopeAttribute != nameSpaceID)
      {
        d->faultCode = VersionMismatch;
        d->faultString = "Wrong nameSpace";
        return false;
      }
    }

    QString encodingStyle = envelope.attribute(d->nameSpace + ":encodingStyle");

    if (!!encodingStyle)
    {
      if (soapEncodingAttribute != encodingStyle)
      {
        d->faultCode = Client;
        d->faultString = "Unknown encoding";
        return false;
      }
    }

    return parseEnvelopeContents(envelope);
  }

    bool
  Parser::parseEnvelopeContents(const QDomElement & envelope)
  {
    QDomNode node = envelope.firstChild();

    while (!node.isNull())
    {
      QDomElement e = node.toElement();

      if (e.isNull() && (!d->haveHeader || !d->haveBody))
      {
        d->faultCode = Client;
        d->faultString = "Bad envelope structure";
        return false;
      }

      if ((d->nameSpace + ":Header") == e.tagName())
      {
        if (!parseHeader(e))
          return false;

        if (!d->authorized)
          return false;
      }
      else if ((d->nameSpace + ":Body") == e.tagName())
      {
        return parseBody(e);
      }
      else
      {
        if (!d->haveHeader || !d->haveBody)
        {
          d->faultCode = Client;
          d->faultString = "Bad envelope structure";
          return false;
        }
      }

      node = node.nextSibling();
    }

    d->faultCode = Client;
    d->faultString = "Bad envelope structure";
    return false;
  }

    bool
  Parser::parseHeader(const QDomElement & e)
  {
    if (d->haveHeader)
    {
      d->faultCode = Client;
      d->faultString = "Multiple Header elements";
      return false;
    }

    QDomNode node = e.firstChild();

    while (!node.isNull())
    {
      QDomElement e = node.toElement();

      if (e.isNull())
      {
        node = node.nextSibling();
        continue;
      }

      if ("dcop:Authorization" == e.tagName())
      {
        QString nameSpaceID = e.attribute("xmlns:dcop");

        if (dcopAttribute != nameSpaceID)
        {
          d->faultCode = Client;
          d->faultString = "Authorization method unknown";
          return false;
        }

        if (e.text() == d->authKey)
        {
          d->authorized = true;
        }
        else
        {
          d->faultCode = Client;
          d->faultString = "Bad authorization key";
          return false;
        }
      }
      else
      {
        bool mustUnderstand = 
          (0 != e.attribute(d->nameSpace + ":mustUnderstand").toUInt());

        if (mustUnderstand)
        {
          d->faultCode = MustUnderstand;
          d->faultString = "Failed to understand mustUnderstand element";
          return false;
        }
      }

      node = node.nextSibling();
    }

    return true;
  }

    bool
  Parser::parseBody(const QDomElement & e)
  {
    if (d->haveBody)
    {
      d->faultCode = Client;
      d->faultString = "Multiple Body elements";
      d->faultDetail = d->faultString;
      return false;
    }

    QDomNode node = e.firstChild();

    while (!node.isNull())
    {
      QDomElement e = node.toElement();

      if (!e.isNull() && ("dcop:Call" == e.tagName()))
      {
        QString nameSpaceID = e.attribute("xmlns:dcop");

        if (dcopAttribute != nameSpaceID)
        {
          d->faultCode = Client;
          d->faultString = "Not a DCOP call";
          d->faultDetail = d->faultString;
          return false;
        }

        if (!getApplication(e))
          return false;

        if (!getObject(e))
          return false;

        if (!getMethod(e))
          return false;
      }

      node = node.nextSibling();
    }

    return true;
  }

    bool
  Parser::getApplication(const QDomElement & e)
  {
    QDomNodeList applicationList(e.elementsByTagName("application"));

    if (applicationList.count() != 1)
    {
      d->faultCode = Client;
      d->faultString = "More than one application specified";
      d->faultDetail = d->faultString;
      return false;
    }

    QDomElement applicationElement(applicationList.item(0).toElement());

    if (applicationElement.isNull())
    {
      d->faultCode = Client;
      d->faultString = "Internal error";
      d->faultDetail = "Node advertised as Element is not";
      return false;
    }

    QString application = applicationElement.text();

    if (application.isEmpty())
    {
      d->faultCode = Client;
      d->faultString = "Application element contains no text";
      d->faultDetail = d->faultString;
      return false;
    }

    d->application = application.utf8();

    return (!d->application.isEmpty());
  }

    bool
  Parser::getObject(const QDomElement & e)
  {
    QDomNodeList objectList(e.elementsByTagName("object"));

    if (objectList.count() != 1)
    {
      d->faultCode = Client;
      d->faultString = "More than one object specified";
      d->faultDetail = d->faultString;
      return false;
    }

    QDomElement objectElement(objectList.item(0).toElement());

    if (objectElement.isNull())
    {
      d->faultCode = Client;
      d->faultString = "Internal error";
      d->faultDetail = "Node advertised as Element is not";
      return false;
    }

    QString object = objectElement.text();

    if (object.isEmpty())
    {
      d->faultCode = Client;
      d->faultString = "Object element contains no text";
      d->faultDetail = d->faultString;
      return false;
    }

    d->object = object.utf8();

    return (!d->object.isEmpty());
  }

    bool
  Parser::getMethod(const QDomElement & e)
  {
    QDomNodeList methodList(e.elementsByTagName("method"));

    if (methodList.count() != 1)
    {
      d->faultCode = Client;
      d->faultString = "More than one method in dcop:Call";
      d->faultDetail = d->faultString;
      return false;
    }

    QDomElement methodElement(methodList.item(0).toElement());

    if (methodElement.isNull())
    {
      d->faultCode = Client;
      d->faultString = "Internal error";
      d->faultDetail = "Node advertised as Element is not";
      return false;
    }

    if (!getMethodName(methodElement))
      return false;

    return getParameters(methodElement);
  }

    bool
  Parser::getMethodName(const QDomElement & e)
  {
    QDomNodeList nameList(e.elementsByTagName("name"));

    if (nameList.count() != 1)
    {
      d->faultCode = Client;
      d->faultString = "More than one name in method";
      d->faultDetail = d->faultString;
      return false;
    }

    QDomElement nameElement(nameList.item(0).toElement());

    if (nameElement.isNull())
    {
      d->faultCode = Client;
      d->faultString = "Internal error";
      d->faultDetail = "Node advertised as Element is not";
      return false;
    }

    QString method = nameElement.text();

    if (method.isEmpty())
    {
      d->faultCode = Client;
      d->faultString = "Method name is empty";
      d->faultDetail = d->faultString;
      return false;
    }

    d->method = method.utf8();

    return (!d->method.isEmpty());
  }

    bool
  Parser::getParameters(const QDomElement & e)
  {
    d->paramTypeList.clear();
    d->data.truncate(0);

    QDataStream str(d->data, IO_WriteOnly);

    QDomNodeList objectList(e.elementsByTagName("param"));

    for (uint i = 0; i < objectList.count(); i++)
    {
      QDomElement parameterElement(objectList.item(i).toElement());

      if (parameterElement.isNull())
      {
        d->faultCode = Client;
        d->faultString = "Node with tag `param' is not element";
        d->faultDetail = "Node advertised as Element is not";
        return false;
      }

      QDomNodeList typeNodeList = parameterElement.elementsByTagName("type");

      if (typeNodeList.count() != 1)
      {
        d->faultCode = Client;
        d->faultString = "type node count is not 1";
        d->faultDetail = d->faultString;
        return false;
      }

      QDomElement typeElement(typeNodeList.item(0).toElement());

      if (typeElement.isNull())
      {
        d->faultCode = Server;
        d->faultString = "Internal error";
        d->faultDetail = "Node advertised as Element is not";
        return false;
      }

      QCString type = typeElement.text().utf8();

      if (type.isEmpty())
      {
        d->faultCode = Client;
        d->faultString = "type is empty";
        d->faultDetail = d->faultString;
        return false;
      }

      QDomNodeList valueNodeList = parameterElement.elementsByTagName("value");

      if (valueNodeList.count() != 1)
      {
        d->faultCode = Client;
        d->faultString = "value node count is not 1";
        d->faultDetail = d->faultString;
        return false;
      }

      QDomElement valueElement(valueNodeList.item(0).toElement());

      if (valueElement.isNull())
      {
        d->faultCode = Server;
        d->faultString = "Internal error";
        d->faultDetail = "Node advertised as Element is not";
        return false;
      }

      QString value = valueElement.text();

      bool ok = marshal(str, type, value);

      if (!ok)
      {
        d->faultCode = Client;
        d->faultString = "Parameter type not understood";
        d->faultDetail = d->faultString;
        return false;
      }
    }

    return true;
  }

    bool
  Parser::marshal
  (
   QDataStream & str,
   const QString & type,
   const QString & value
  )
  {
    bool ret = false;

    if ("" == type || "string" == type)
    {
      d->paramTypeList << "QString";

      str << value;

      ret = true;
    }
    else if ("boolean" == type)
    {
      d->paramTypeList << "bool";

      str << (("true" == value || "1" == value) ? true : false);

      ret = true;
    }
    else if ("int" == type)
    {
      d->paramTypeList << "int";

      str << value.toInt();

      ret = true;
    }
    else if ("decimal" == type)
    {
      d->paramTypeList << "float";

      str << value.toDouble();

      ret = true;
    }
    else if ("float" == type)
    {
      d->paramTypeList << "float";

      str << value.toFloat();

      ret = true;
    }
    else if ("double" == type)
    {
      d->paramTypeList << "double";

      str << value.toDouble();

      ret = true;
    }
    else
    {
      qDebug("Don't understand type `%s'", type.utf8().data());
    }

    return ret;
  }

    QString
  Parser::elementName(const QString & nameSpaceAndName)
  {
    int nameSpaceEnd = nameSpaceAndName.find(':');

    if (-1 == nameSpaceEnd)
      return nameSpaceAndName;
    else
      return nameSpaceAndName.mid(nameSpaceEnd + 1);
  }

    QString
  Parser::elementNamespace(const QString & nameSpaceAndName)
  {
    int nameSpaceEnd = nameSpaceAndName.find(':');

    if (-1 == nameSpaceEnd)
      return QString::null;
    else
      return nameSpaceAndName.left(nameSpaceEnd);
  }

} // End namespace SODA


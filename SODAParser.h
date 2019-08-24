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

#ifndef SODA_PARSER_H
#define SODA_PARSER_H

#include <qstring.h>
#include <qmap.h>
#include <qcstring.h>
#include <qvaluelist.h>

class QDomElement;

typedef QValueList<QCString> QCStringList;

namespace SODA
{
  class Parser
  {
    public:

      Parser(const QDomElement & e, const QString & authKey);
      virtual ~Parser();

      bool getCall
        (
         QCString & app,
         QCString & object,
         QCString & method,
         QByteArray & data
        );

      enum FaultCode
      {
        NoFault,
        VersionMismatch,
        MustUnderstand,
        Client,
        Server
      };

      FaultCode faultCode()   const;
      QString   faultString() const;
      QString   faultDetail() const;

    protected:

      bool parseEnvelope          (const QDomElement & envelope);
      bool parseEnvelopeContents  (const QDomElement & envelope);
      bool parseHeader            (const QDomElement & e);
      bool parseBody              (const QDomElement & e);

      bool  getApplication        (const QDomElement & e);
      bool  getObject             (const QDomElement & e);
      bool  getMethod             (const QDomElement & e);
      bool  getMethodName         (const QDomElement & e);
      bool  getParameters         (const QDomElement & e);

      QString elementName         (const QString & nameSpaceAndName);
      QString elementNamespace    (const QString & nameSpaceAndName);

      bool  marshal(QDataStream &, const QString & type, const QString & value);

    private:

      // Disable copying.
      Parser(const Parser &);
      Parser & operator = (const Parser &);

      class Private;
      Private * d;
  };

} // End namespace SODA

#endif

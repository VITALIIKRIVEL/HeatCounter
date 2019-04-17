#ifndef HTTP_H
#define HTTP_H

#include <QString>

namespace nayk {
//======================================================================================================
const QString ContentTypeImageGIF             = "image/gif";
const QString ContentTypeImageJPEG            = "image/jpeg";
const QString ContentTypeImageJPG             = "image/jpg";
const QString ContentTypeImagePNG             = "image/png";
const QString ContentTypeImageSVG             = "image/svg+xml";
const QString ContentTypeImageTIFF            = "image/tiff";
const QString ContentTypeFont                 = "application/font";
const QString ContentTypeFontEOT              = "application/vnd.ms-fontobject";
const QString ContentTypeFontOTF              = "font/opentype";
const QString ContentTypeFontTTF              = "font/ttf";
const QString ContentTypeFontWOFF             = "font/woff";
const QString ContentTypeScriptECMAS          = "text/ecmascript";
const QString ContentTypeScriptVB             = "text/vbscript";
const QString ContentTypeCSS                  = "text/css";
const QString ContentTypeHTML                 = "text/html";
const QString ContentTypeJS                   = "text/javascript";
const QString ContentTypeJSON                 = "application/json";
const QString ContentTypeText                 = "text/plain";
const QString ContentTypeXML                  = "text/xml";
const QString ContentTypeWWWForm              = "application/x-www-form-urlencoded";
const QString ContentTypeMultipartForm        = "multipart/form-data";
const QString ContentTypeBinary               = "application/octet-stream";
// Server headers:
const QString ServerHeaderComSpec             = "COMSPEC";
const QString ServerHeaderDocumentRoot        = "DOCUMENT_ROOT";
const QString ServerHeaderGatewayInterface    = "GATEWAY_INTERFACE";
const QString ServerHeaderHttpAccept          = "HTTP_ACCEPT";
const QString ServerHeaderHttpAcceptEncoding  = "HTTP_ACCEPT_ENCODING";
const QString ServerHeaderHttpAcceptLanguage  = "HTTP_ACCEPT_LANGUAGE";
const QString ServerHeaderHttpConnection      = "HTTP_CONNECTION";
const QString ServerHeaderHttpCookie          = "HTTP_COOKIE";
const QString ServerHeaderHttpHost            = "HTTP_HOST";
const QString ServerHeaderHttpUserAgent       = "HTTP_USER_AGENT";
const QString ServerHeaderHttps               = "HTTPS";
const QString ServerHeaderPath                = "PATH";
const QString ServerHeaderQueryString         = "QUERY_STRING";
const QString ServerHeaderRemoteAddress       = "REMOTE_ADDR";
const QString ServerHeaderRemotePort          = "REMOTE_PORT";
const QString ServerHeaderRequestMethod       = "REQUEST_METHOD";
const QString ServerHeaderRequestScheme       = "REQUEST_SCHEME";
const QString ServerHeaderRequestUri          = "REQUEST_URI";
const QString ServerHeaderScriptFilename      = "SCRIPT_FILENAME";
const QString ServerHeaderScriptName          = "SCRIPT_NAME";
const QString ServerHeaderServerAddress       = "SERVER_ADDR";
const QString ServerHeaderServerAdministrator = "SERVER_ADMIN";
const QString ServerHeaderServerName          = "SERVER_NAME";
const QString ServerHeaderServerPort          = "SERVER_PORT";
const QString ServerHeaderServerProtocol      = "SERVER_PROTOCOL";
const QString ServerHeaderServerSignature     = "SERVER_SIGNATURE";
const QString ServerHeaderServerSoftware      = "SERVER_SOFTWARE";
const QString ServerHeaderContentType         = "CONTENT_TYPE";
const QString ServerHeaderContentLength       = "CONTENT_LENGTH";
const QString ServerHeaderContextPrefix       = "CONTEXT_PREFIX";
//
const QString MethodDelete                    = "DELETE";
const QString MethodGet                       = "GET";
const QString MethodHead                      = "HEAD";
const QString MethodPost                      = "POST";
const QString MethodPut                       = "PUT";
// Response headers:
const QString HeaderContentLength             = "Content-Length";
const QString HeaderContentType               = "Content-Type";
const QString HeaderCookie                    = "Set-Cookie";

//======================================================================================================
} // namespace nayk
#endif // HTTP_H

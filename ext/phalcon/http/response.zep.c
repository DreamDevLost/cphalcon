
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/file.h"


/*
 +------------------------------------------------------------------------+
 | Phalcon Framework                                                      |
 +------------------------------------------------------------------------+
 | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
 +------------------------------------------------------------------------+
 | This source file is subject to the New BSD License that is bundled     |
 | with this package in the file docs/LICENSE.txt.                        |
 |                                                                        |
 | If you did not receive a copy of the license and are unable to         |
 | obtain it through the world-wide-web, please send an email             |
 | to license@phalconphp.com so we can send you a copy immediately.       |
 +------------------------------------------------------------------------+
 | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
 |          Eduar Carvajal <eduar@phalconphp.com>                         |
 +------------------------------------------------------------------------+
 */
/**
 * Phalcon\Http\Response
 *
 * Part of the HTTP cycle is return responses to the clients.
 * Phalcon\HTTP\Response is the Phalcon component responsible to achieve this task.
 * HTTP responses are usually composed by headers and body.
 *
 *<code>
 *	$response = new \Phalcon\Http\Response();
 *	$response->setStatusCode(200, "OK");
 *	$response->setContent("<html><body>Hello</body></html>");
 *	$response->send();
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Http_Response) {

	ZEPHIR_REGISTER_CLASS(Phalcon\\Http, Response, phalcon, http_response, phalcon_http_response_method_entry, 0);

	zend_declare_property_bool(phalcon_http_response_ce, SL("_sent"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_http_response_ce, SL("_content"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_http_response_ce, SL("_headers"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_http_response_ce, SL("_cookies"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_http_response_ce, SL("_file"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_http_response_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(phalcon_http_response_ce TSRMLS_CC, 1, phalcon_http_responseinterface_ce);
	zend_class_implements(phalcon_http_response_ce TSRMLS_CC, 1, phalcon_di_injectionawareinterface_ce);
	return SUCCESS;

}

/**
 * Phalcon\Http\Response constructor
 *
 * @param string content
 * @param int code
 * @param string status
 */
PHP_METHOD(Phalcon_Http_Response, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *content = NULL, *code = NULL, *status = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 3, &content, &code, &status);

	if (!content) {
		content = ZEPHIR_GLOBAL(global_null);
	}
	if (!code) {
		code = ZEPHIR_GLOBAL(global_null);
	}
	if (!status) {
		status = ZEPHIR_GLOBAL(global_null);
	}


	if (Z_TYPE_P(content) != IS_NULL) {
		zephir_update_property_this(this_ptr, SL("_content"), content TSRMLS_CC);
	}
	if (Z_TYPE_P(code) != IS_NULL) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "setstatuscode", NULL, code, status);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Sets the dependency injector
 *
 * @param Phalcon\DiInterface dependencyInjector
 */
PHP_METHOD(Phalcon_Http_Response, setDI) {

	zval *dependencyInjector;

	zephir_fetch_params(0, 1, 0, &dependencyInjector);



	if (!(zephir_instance_of_ev(dependencyInjector, phalcon_diinterface_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Parameter 'dependencyInjector' must be an instance of 'Phalcon\\DiInterface'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);

}

/**
 * Returns the internal dependency injector
 *
 * @return Phalcon\DiInterface
 */
PHP_METHOD(Phalcon_Http_Response, getDI) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL;
	zval *dependencyInjector = NULL, *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(dependencyInjector, _0);
	if (Z_TYPE_P(dependencyInjector) != IS_OBJECT) {
		ZEPHIR_CALL_CE_STATIC(&dependencyInjector, phalcon_di_ce, "getdefault", &_1);
		zephir_check_call_status();
		if (Z_TYPE_P(dependencyInjector) != IS_OBJECT) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_http_request_exception_ce, "A dependency injection object is required to access the 'url' service", "phalcon/http/response.zep", 90);
			return;
		}
		zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);
	}
	RETURN_CCTOR(dependencyInjector);

}

/**
 * Sets the HTTP response code
 *
 *<code>
 *	$response->setStatusCode(404, "Not Found");
 *</code>
 *
 * @param int code
 * @param string message
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setStatusCode) {

	zephir_nts_static zephir_fcall_cache_entry *_6 = NULL;
	zend_bool _3;
	HashTable *_1;
	HashPosition _0;
	zval *message = NULL, *_7, *_9;
	zval *code_param = NULL, *message_param = NULL, *headers = NULL, *currentHeadersRaw = NULL, *key = NULL, *value = NULL, **_2, _4 = zval_used_for_init, *_5 = NULL, _8, *_10;
	int code, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &code_param, &message_param);

	code = zephir_get_intval(code_param);
	zephir_get_strval(message, message_param);


	ZEPHIR_CALL_METHOD(&headers, this_ptr, "getheaders",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&currentHeadersRaw, headers, "toarray",  NULL);
	zephir_check_call_status();
	if (Z_TYPE_P(currentHeadersRaw) == IS_ARRAY) {
		zephir_is_iterable(currentHeadersRaw, &_1, &_0, 0, 0);
		for (
		  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		  ; zephir_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HMKEY(key, _1, _0);
			ZEPHIR_GET_HVALUE(value, _2);
			_3 = Z_TYPE_P(key) == IS_STRING;
			if (_3) {
				ZEPHIR_SINIT_NVAR(_4);
				ZVAL_STRING(&_4, "HTTP/", 0);
				ZEPHIR_CALL_FUNCTION(&_5, "strstr", &_6, key, &_4);
				zephir_check_call_status();
				_3 = zephir_is_true(_5);
			}
			if (_3) {
				ZEPHIR_CALL_METHOD(NULL, headers, "remove", NULL, key);
				zephir_check_call_status();
			}
		}
	}
	ZEPHIR_SINIT_NVAR(_4);
	ZVAL_LONG(&_4, code);
	ZEPHIR_INIT_VAR(_7);
	ZEPHIR_CONCAT_SVSV(_7, "HTTP/1.1 ", &_4, " ", message);
	ZEPHIR_CALL_METHOD(NULL, headers, "setraw", NULL, _7);
	zephir_check_call_status();
	ZEPHIR_SINIT_VAR(_8);
	ZVAL_LONG(&_8, code);
	ZEPHIR_INIT_VAR(_9);
	ZEPHIR_CONCAT_VSV(_9, &_8, " ", message);
	ZEPHIR_INIT_VAR(_10);
	ZVAL_STRING(_10, "Status", 0);
	ZEPHIR_CALL_METHOD(NULL, headers, "set", NULL, _10, _9);
	zephir_check_temp_parameter(_10);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("_headers"), headers TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Sets a headers bag for the response externally
 *
 * @param Phalcon\Http\Response\HeadersInterface headers
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setHeaders) {

	zval *headers;

	zephir_fetch_params(0, 1, 0, &headers);



	zephir_update_property_this(this_ptr, SL("_headers"), headers TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Returns headers set by the user
 *
 * @return Phalcon\Http\Response\HeadersInterface
 */
PHP_METHOD(Phalcon_Http_Response, getHeaders) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *headers;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(headers);
	zephir_read_property_this(&headers, this_ptr, SL("_headers"), PH_NOISY_CC);
	if (Z_TYPE_P(headers) == IS_NULL) {
		ZEPHIR_INIT_BNVAR(headers);
		object_init_ex(headers, phalcon_http_response_headers_ce);
		if (zephir_has_constructor(headers TSRMLS_CC)) {
			ZEPHIR_CALL_METHOD(NULL, headers, "__construct", NULL);
			zephir_check_call_status();
		}
		zephir_update_property_this(this_ptr, SL("_headers"), headers TSRMLS_CC);
	}
	RETURN_CCTOR(headers);

}

/**
 * Sets a cookies bag for the response externally
 *
 * @param Phalcon\Http\Response\CookiesInterface cookies
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setCookies) {

	zval *cookies;

	zephir_fetch_params(0, 1, 0, &cookies);



	if (!(zephir_instance_of_ev(cookies, phalcon_http_response_cookiesinterface_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Parameter 'cookies' must be an instance of 'Phalcon\\Http\\Response\\CookiesInterface'", "", 0);
		return;
	}
	if (Z_TYPE_P(cookies) != IS_OBJECT) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(phalcon_http_response_exception_ce, "The cookies bag is not valid", "phalcon/http/response.zep", 179);
		return;
	}
	zephir_update_property_this(this_ptr, SL("_cookies"), cookies TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Returns coookies set by the user
 *
 * @return Phalcon\Http\Response\CookiesInterface
 */
PHP_METHOD(Phalcon_Http_Response, getCookies) {


	RETURN_MEMBER(this_ptr, "_cookies");

}

/**
 * Overwrites a header in the response
 *
 *<code>
 *	$response->setHeader("Content-Type", "text/plain");
 *</code>
 *
 * @param string name
 * @param string value
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setHeader) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *name_param = NULL, *value, *headers = NULL;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &name_param, &value);

	zephir_get_strval(name, name_param);


	ZEPHIR_CALL_METHOD(&headers, this_ptr, "getheaders",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, headers, "set", NULL, name, value);
	zephir_check_call_status();
	RETURN_THIS();

}

/**
 * Send a raw header to the response
 *
 *<code>
 *	$response->setRawHeader("HTTP/1.1 404 Not Found");
 *</code>
 *
 * @param string header
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setRawHeader) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *header_param = NULL, *headers = NULL;
	zval *header = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &header_param);

	zephir_get_strval(header, header_param);


	ZEPHIR_CALL_METHOD(&headers, this_ptr, "getheaders",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, headers, "setraw", NULL, header);
	zephir_check_call_status();
	RETURN_THIS();

}

/**
 * Resets all the stablished headers
 *
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, resetHeaders) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *headers = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&headers, this_ptr, "getheaders",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, headers, "reset", NULL);
	zephir_check_call_status();
	RETURN_THIS();

}

/**
 * Sets a Expires header to use HTTP cache
 *
 *<code>
 *	$this->response->setExpires(new DateTime());
 *</code>
 *
 * @param DateTime datetime
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setExpires) {

	zend_class_entry *_1;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *datetime, *headers = NULL, *date, *_0, *_2, *_3 = NULL, *_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &datetime);



	if (!(zephir_is_instance_of(datetime, SL("DateTime") TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'datetime' must be an instance of 'DateTime'", "", 0);
		return;
	}
	ZEPHIR_CALL_METHOD(&headers, this_ptr, "getheaders",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(date);
	if (zephir_clone(date, datetime TSRMLS_CC) == FAILURE) {
		RETURN_MM();
	}
	ZEPHIR_INIT_VAR(_0);
	_1 = zend_fetch_class(SL("DateTimeZone"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(_0, _1);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "UTC", 0);
	ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, _2);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, date, "settimezone", NULL, _0);
	zephir_check_call_status();
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "D, d M Y H:i:s", 0);
	ZEPHIR_CALL_METHOD(&_3, date, "format", NULL, _2);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_4);
	ZEPHIR_CONCAT_VS(_4, _3, " GMT");
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "Expires", 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "setheader", NULL, _2, _4);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	RETURN_THIS();

}

/**
 * Sends a Not-Modified response
 *
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setNotModified) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 304);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "Not modified", 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "setstatuscode", NULL, _0, _1);
	zephir_check_temp_parameter(_1);
	zephir_check_call_status();
	RETURN_THIS();

}

/**
 * Sets the response content-type mime, optionally the charset
 *
 *<code>
 *	$response->setContentType('application/pdf');
 *	$response->setContentType('text/plain', 'UTF-8');
 *</code>
 *
 * @param string contentType
 * @param string charset
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setContentType) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *contentType_param = NULL, *charset = NULL, *headers = NULL, *name, *_0;
	zval *contentType = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &contentType_param, &charset);

	zephir_get_strval(contentType, contentType_param);
	if (!charset) {
		charset = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_CALL_METHOD(&headers, this_ptr, "getheaders",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(name);
	ZVAL_STRING(name, "Content-Type", 1);
	if (Z_TYPE_P(charset) == IS_NULL) {
		ZEPHIR_CALL_METHOD(NULL, headers, "set", NULL, name, contentType);
		zephir_check_call_status();
	} else {
		ZEPHIR_INIT_VAR(_0);
		ZEPHIR_CONCAT_VSV(_0, contentType, "; charset=", charset);
		ZEPHIR_CALL_METHOD(NULL, headers, "set", NULL, name, _0);
		zephir_check_call_status();
	}
	RETURN_THIS();

}

/**
 * Set a custom ETag
 *
 *<code>
 *	$response->setEtag(md5(time()));
 *</code>
 *
 * @param string etag
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setEtag) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *etag_param = NULL, *headers = NULL, *_0;
	zval *etag = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &etag_param);

	zephir_get_strval(etag, etag_param);


	ZEPHIR_CALL_METHOD(&headers, this_ptr, "getheaders",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "Etag", 0);
	ZEPHIR_CALL_METHOD(NULL, headers, "set", NULL, _0, etag);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_THIS();

}

/**
 * Redirect by HTTP to another action or URL
 *
 *<code>
 *  //Using a string redirect (internal/external)
 *	$response->redirect("posts/index");
 *	$response->redirect("http://en.wikipedia.org", true);
 *	$response->redirect("http://www.example.com/new-location", true, 301);
 *
 *	//Making a redirection based on a named route
 *	$response->redirect(array(
 *		"for" => "index-lang",
 *		"lang" => "jp",
 *		"controller" => "index"
 *	));
 *</code>
 *
 * @param string|array location
 * @param boolean externalRedirect
 * @param int statusCode
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, redirect) {

	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL, *_4 = NULL;
	zend_bool _0;
	int statusCode, ZEPHIR_LAST_CALL_STATUS;
	zval *location = NULL, *externalRedirect = NULL, *statusCode_param = NULL, *header = NULL, *url = NULL, *dependencyInjector = NULL, *messages, *matched = NULL, *message = NULL, _1 = zval_used_for_init, *_2 = NULL, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 3, &location, &externalRedirect, &statusCode_param);

	if (!location) {
		ZEPHIR_CPY_WRT(location, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(location);
	}
	if (!externalRedirect) {
		externalRedirect = ZEPHIR_GLOBAL(global_false);
	}
	if (!statusCode_param) {
		statusCode = 302;
	} else {
		statusCode = zephir_get_intval(statusCode_param);
	}


	ZEPHIR_INIT_VAR(messages);
	array_init_size(messages, 13);
	add_index_stringl(messages, 300, SL("Multiple Choices"), 1);
	add_index_stringl(messages, 301, SL("Moved Permanently"), 1);
	add_index_stringl(messages, 302, SL("Found"), 1);
	add_index_stringl(messages, 303, SL("See Other"), 1);
	add_index_stringl(messages, 304, SL("Not Modified"), 1);
	add_index_stringl(messages, 305, SL("Use Proxy"), 1);
	add_index_stringl(messages, 306, SL("Switch Proxy"), 1);
	add_index_stringl(messages, 307, SL("Temporary Redirect"), 1);
	add_index_stringl(messages, 308, SL("Permanent Redirect"), 1);
	if (!(zephir_is_true(location))) {
		ZEPHIR_INIT_NVAR(location);
		ZVAL_STRING(location, "", 1);
	}
	if (zephir_is_true(externalRedirect)) {
		ZEPHIR_CPY_WRT(header, location);
	} else {
		_0 = Z_TYPE_P(location) == IS_STRING;
		if (_0) {
			ZEPHIR_SINIT_VAR(_1);
			ZVAL_STRING(&_1, "://", 0);
			ZEPHIR_CALL_FUNCTION(&_2, "strstr", &_3, location, &_1);
			zephir_check_call_status();
			_0 = zephir_is_true(_2);
		}
		if (_0) {
			ZEPHIR_SINIT_NVAR(_1);
			ZVAL_STRING(&_1, "/^[^:\\/?#]++:/", 0);
			ZEPHIR_CALL_FUNCTION(&matched, "preg_match", &_4, &_1, location);
			zephir_check_call_status();
			if (zephir_is_true(matched)) {
				ZEPHIR_CPY_WRT(header, location);
			} else {
				ZEPHIR_INIT_VAR(header);
				ZVAL_NULL(header);
			}
		} else {
			ZEPHIR_INIT_NVAR(header);
			ZVAL_NULL(header);
		}
	}
	if (!(zephir_is_true(header))) {
		ZEPHIR_CALL_METHOD(&dependencyInjector, this_ptr, "getdi",  NULL);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_5);
		ZVAL_STRING(_5, "url", 0);
		ZEPHIR_CALL_METHOD(&_2, dependencyInjector, "getshared", NULL, _5);
		zephir_check_temp_parameter(_5);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(url, _2);
		ZEPHIR_CALL_METHOD(&header, url, "get", NULL, location);
		zephir_check_call_status();
	}
	_0 = statusCode < 300;
	if (!(_0)) {
		_0 = statusCode > 308;
	}
	if (_0) {
		statusCode = 302;
		ZEPHIR_INIT_VAR(message);
		ZVAL_STRING(message, "Redirect", 1);
	} else {
		ZEPHIR_OBS_NVAR(message);
		zephir_array_fetch_long(&message, messages, statusCode, PH_NOISY, "phalcon/http/response.zep", 399 TSRMLS_CC);
	}
	ZEPHIR_INIT_NVAR(_5);
	ZVAL_LONG(_5, statusCode);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "setstatuscode", NULL, _5, message);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(_5);
	ZVAL_STRING(_5, "Location", 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "setheader", NULL, _5, header);
	zephir_check_temp_parameter(_5);
	zephir_check_call_status();
	RETURN_THIS();

}

/**
 * Sets HTTP response body
 *
 *<code>
 *	response->setContent("<h1>Hello!</h1>");
 *</code>
 *
 * @param string content
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setContent) {

	zval *content_param = NULL;
	zval *content = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &content_param);

	zephir_get_strval(content, content_param);


	zephir_update_property_this(this_ptr, SL("_content"), content TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Sets HTTP response body. The parameter is automatically converted to JSON
 *
 *<code>
 *	$response->setJsonContent(array("status" => "OK"));
 *</code>
 *
 * @param mixed content
 * @param int jsonOptions
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setJsonContent) {

	zval *content, *jsonOptions = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &content, &jsonOptions);

	if (!jsonOptions) {
		ZEPHIR_INIT_VAR(jsonOptions);
		ZVAL_LONG(jsonOptions, 0);
	}


	ZEPHIR_INIT_VAR(_0);
	zephir_json_encode(_0, &(_0), content, zephir_get_intval(jsonOptions)  TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_content"), _0 TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Appends a string to the HTTP response body
 *
 * @param string content
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, appendContent) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *content, *_0 = NULL, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &content);



	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getcontent",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_1);
	ZEPHIR_CONCAT_VV(_1, _0, content);
	zephir_update_property_this(this_ptr, SL("_content"), _1 TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Gets the HTTP response body
 *
 * @return string
 */
PHP_METHOD(Phalcon_Http_Response, getContent) {


	RETURN_MEMBER(this_ptr, "_content");

}

/**
 * Check if the response is already sent
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Http_Response, isSent) {


	RETURN_MEMBER(this_ptr, "_sent");

}

/**
 * Sends headers to the client
 *
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, sendHeaders) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *headers;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(headers);
	zephir_read_property_this(&headers, this_ptr, SL("_headers"), PH_NOISY_CC);
	if (Z_TYPE_P(headers) == IS_OBJECT) {
		ZEPHIR_CALL_METHOD(NULL, headers, "send", NULL);
		zephir_check_call_status();
	}
	RETURN_THIS();

}

/**
 * Sends cookies to the client
 *
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, sendCookies) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *cookies;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(cookies);
	zephir_read_property_this(&cookies, this_ptr, SL("_cookies"), PH_NOISY_CC);
	if (Z_TYPE_P(cookies) == IS_OBJECT) {
		ZEPHIR_CALL_METHOD(NULL, cookies, "send", NULL);
		zephir_check_call_status();
	}
	RETURN_THIS();

}

/**
 * Prints out HTTP response to the client
 *
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, send) {

	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zend_bool _1;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *headers, *cookies, *content, *file, *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_sent"), PH_NOISY_CC);
	if (zephir_is_true(_0)) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_http_response_exception_ce, "Response was already sent", "phalcon/http/response.zep", 517);
		return;
	}
	ZEPHIR_OBS_VAR(headers);
	zephir_read_property_this(&headers, this_ptr, SL("_headers"), PH_NOISY_CC);
	if (Z_TYPE_P(headers) == IS_OBJECT) {
		ZEPHIR_CALL_METHOD(NULL, headers, "send", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_OBS_VAR(cookies);
	zephir_read_property_this(&cookies, this_ptr, SL("_cookies"), PH_NOISY_CC);
	if (Z_TYPE_P(cookies) == IS_OBJECT) {
		ZEPHIR_CALL_METHOD(NULL, cookies, "send", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_OBS_VAR(content);
	zephir_read_property_this(&content, this_ptr, SL("_content"), PH_NOISY_CC);
	if (Z_TYPE_P(content) != IS_NULL) {
		zend_print_zval(content, 0);
	} else {
		ZEPHIR_OBS_VAR(file);
		zephir_read_property_this(&file, this_ptr, SL("_file"), PH_NOISY_CC);
		_1 = Z_TYPE_P(file) == IS_STRING;
		if (_1) {
			_1 = (zephir_fast_strlen_ev(file)) ? 1 : 0;
		}
		if (_1) {
			ZEPHIR_CALL_FUNCTION(NULL, "readfile", &_2, file);
			zephir_check_call_status();
		}
	}
	zephir_update_property_this(this_ptr, SL("_sent"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Sets an attached file to be sent at the end of the request
 *
 * @param string filePath
 * @param string attachmentName
 * @return Phalcon\Http\ResponseInterface
 */
PHP_METHOD(Phalcon_Http_Response, setFileToSend) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *filePath_param = NULL, *attachmentName = NULL, *attachment = NULL, *basePath = NULL, *headers = NULL, *_0 = NULL, *_1;
	zval *filePath = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &filePath_param, &attachmentName, &attachment);

	zephir_get_strval(filePath, filePath_param);
	if (!attachmentName) {
		attachmentName = ZEPHIR_GLOBAL(global_null);
	}
	if (!attachment) {
		attachment = ZEPHIR_GLOBAL(global_true);
	}


	if (Z_TYPE_P(attachmentName) != IS_STRING) {
		ZEPHIR_INIT_VAR(basePath);
		zephir_basename(basePath, filePath TSRMLS_CC);
	} else {
		ZEPHIR_CPY_WRT(basePath, attachmentName);
	}
	if (zephir_is_true(attachment)) {
		ZEPHIR_CALL_METHOD(&headers, this_ptr, "getheaders",  NULL);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_0);
		ZVAL_STRING(_0, "Content-Description: File Transfer", 0);
		ZEPHIR_CALL_METHOD(NULL, headers, "setraw", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "Content-Type: application/octet-stream", 0);
		ZEPHIR_CALL_METHOD(NULL, headers, "setraw", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_CONCAT_SV(_1, "Content-Disposition: attachment; filename=", basePath);
		ZEPHIR_CALL_METHOD(NULL, headers, "setraw", NULL, _1);
		zephir_check_call_status();
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "Content-Transfer-Encoding: binary", 0);
		ZEPHIR_CALL_METHOD(NULL, headers, "setraw", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	}
	zephir_update_property_this(this_ptr, SL("_file"), filePath TSRMLS_CC);
	RETURN_THIS();

}


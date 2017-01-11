from cgi import parse_qs, escape
from wsgiref.simple_server import make_server
html =""" 
<html>
<body>
	<form method='post' action=''>
	<p>Message: <input type='text' name='msg' value='%(msg)s'></p>
	<p><input type='submit' value='Submit'></p>
	</form>
	<p name='method'>Method: %(method)s</p>
</body>
</html>"""

def application1(env, start_response):
	http_host = 'Http host: ' + env['HTTP_HOST']
	request_method = 'Request method: ' +  env['REQUEST_METHOD']
	#raw_uri = 'Raw uri:' + env['RAW_URI']
	query_string = 'Query string: ' + env['QUERY_STRING']

	data = 'Hi, You\n' + request_method + '\n' + query_string + '\n' + http_host
	
	#GET
	res_template = '{0}: {1}' + ', '
	response_get = 'GET params: '
	for key, value in parse_qs(env['QUERY_STRING']).items():
		response_get += res_template.format(key, value)

	data += '\n' + response_get
	#
	status = '200 OK'
	response_headers = [
		('Content-Type', 'text/plain'),
		('Content-Length', str(len(data)))
	]
	start_response(status, response_headers)
	return data

def application2(env, start_response):
	#POST
	try:
		req_body_size = int(env.get('CONTENT_LENGTH', 0))
	except (ValueError):
		req_body_size = 0
	req_body = env['wsgi.input'].read(req_body_size)
	body = parse_qs(req_body)
	msg = body.get('msg', [''])[0]
	msg = escape(msg)
	method = env['REQUEST_METHOD']
	response_post = html % {
		'msg': msg or 'Empty',
		'method': method + ' ' + msg or 'EMPTY',
	}
	#
	status = '200 OK'
	response_headers = [
		('Content-Type', 'text/html'),
		('Content-Length', str(len(response_post)))
	]
	start_response(status, response_headers)
	return [response_post]

"""httpd = make_server ('localhost', 8081, application2)
httpd.serve_forever()"""

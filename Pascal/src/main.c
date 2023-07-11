#include "ps_assert.h"
#include "string.h"
#include "pascal.h"
#include "ps_response.h"
#include "ps_file.h"

#include <stdio.h>

static const u16 port = 8080;
#define BUFFER_LENGTH 1024

static char receive_buffer[BUFFER_LENGTH];
static char http_response[BUFFER_LENGTH * 2] = "HTTP/1.1 200 OK\r\n\n";
static char http_response_body[BUFFER_LENGTH * 2];
static char http_css_response_body[BUFFER_LENGTH * 2];

static void handle_request(ps_request* request)
{
    //receive_data_from_client(request->client_socket, receive_buffer, BUFFER_LENGTH, 0);

    if(strcmp(request->target, "/style.css") == 0)
    {

    }
    else if(strcmp(request->target, "/") == 0)
    {

    }
    else if(strcmp(request->target, "/favicon.ico") == 0)
    {
        ps_response * response = response_init(request->client_socket);
        ps_respond(response);
    }
}

static void handle_home_request(ps_request* request)
{
    ps_response* response = response_init(request->client_socket);
    response_set_content_type(response, "text/html");
    response_set_body(response, http_response_body, strlen(http_response_body));

    ps_respond(response);
}

static void handle_style_request(ps_request* request)
{
    ps_response* response2 = response_init(request->client_socket);
    response_set_content_type(response2, "text/css");
    response_set_body(response2, http_css_response_body, strlen(http_css_response_body));

    ps_respond(response2);
}

int main(int argc, char** argv)
{
    char* response_data = read_file("Assets/index.html");
    strcat(http_response_body, response_data);

    if(response_data)
        free(response_data);

    response_data = read_file("Assets/style.css");
    strcat(http_css_response_body, response_data);
    if(response_data)
        free(response_data);

    printf("%s\n", http_css_response_body);

    int result = ps_init();
    PS_ASSERT(!result, "Can't start pascal");

    ps_server* server = init_server(PS_TCP);

    server_add_route(server, "/", handle_home_request);
    server_add_route(server, "/style.css", handle_style_request);

    server_listen(server, port, PS_DEFAULT_BACKLOG);

    shutdown_server(server);

    result = ps_shutdown();
    PS_ASSERT(!result, "Can't shutdown pascal");

    return 0;
}

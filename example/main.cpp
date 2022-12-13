#include <iostream>
#include <event.h>
#include <csignal>
#include <cstring>
#include "../3rd/libevent/include/event2/listener.h"

using namespace std;

constexpr unsigned short PORT = 9995;

static void listener_cb(struct evconnlistener *, evutil_socket_t,
                        struct sockaddr *, int socklen, void *);
static void conn_writecb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);
static void signal_cb(evutil_socket_t, short, void *);

static const char MESSAGE[] = "Hello, World!\n";

int main() {
    // 소케 주소를 명명하기 위한 구조체
    struct sockaddr_in sin = {0};

    // 나머지 Libevent와 함께 사용할 새 event_base를 생성
    auto base = event_base_new();

    sin.sin_family = AF_INET;  // AF_INET, AF_INET6, AF_UNIX, AF_LOCAL, AF_LINK, AF_PACKET
    sin.sin_port = htons(PORT);

    // 지정된 주소로 TCP 연결을 수신할 새 evconnlistener 개체 할당
    auto listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
                                            LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
                                            (struct sockaddr*)&sin, sizeof(sin));

    // 시그널 처리를 위한 매크로 함수
    auto signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);
    if (!signal_event || event_add(signal_event, NULL)<0) {
        fprintf(stderr, "Could not create/add a signal event!\n");
        return 1;
    }

    // 이 루프는 보류(pending)이거나 활성(active) 상태가 더 이상 없을 때까지
    // 또는 무언가가 event_base_loopbreak()
    // 또는 event_base_loopexit()를 호출할 때까지 이벤트 기반을 실행
    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);

    printf("done\n");

    return 0;

}


static void
listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
            struct sockaddr *sa, int socklen, void *user_data)
{
    auto *base = static_cast<event_base *>(user_data);
    struct bufferevent *bev;

    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        fprintf(stderr, "Error constructing bufferevent!");
        event_base_loopbreak(base);
        return;
    }
    bufferevent_setcb(bev, NULL, conn_writecb, conn_eventcb, NULL);
    bufferevent_enable(bev, EV_WRITE);
    bufferevent_disable(bev, EV_READ);

    bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

static void
conn_writecb(struct bufferevent *bev, void *user_data)
{
    struct evbuffer *output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) == 0) {
        printf("flushed answer\n");
        bufferevent_free(bev);
    }
}

static void
conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    if (events & BEV_EVENT_EOF) {
        printf("Connection closed.\n");
    } else if (events & BEV_EVENT_ERROR) {
        printf("Got an error on the connection: %s\n",
               strerror(errno));/*XXX win32*/
    }
    /* None of the other events can happen here, since we haven't enabled
     * timeouts */
    bufferevent_free(bev);
}

static void
signal_cb(evutil_socket_t sig, short events, void *user_data)
{
    auto *base = static_cast<event_base *>(user_data);
    struct timeval delay = { 2, 0 };

    printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

    event_base_loopexit(base, &delay);
}
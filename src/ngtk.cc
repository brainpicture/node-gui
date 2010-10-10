#include <v8.h>

#include <ev.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <stdlib.h> // malloc, free

#include "ngtk.h"
#include "ngtk_loop.h"
#include "ngtk_window.h"
#include "ngtk_message_dialog.h"
#include "ngtk_button.h"

namespace ngtk {

using namespace v8;

struct econtext {
  GPollFD *pfd;
  ev_io *iow;
  int nfd, afd;
  gint maxpri;

  ev_prepare pw;
  ev_check cw;
  ev_timer tw;

  GMainContext *gc;
};

static void timer_cb (EV_P_ ev_timer *w, int revents) {
  /* nop */
}

static void io_cb (EV_P_ ev_io *w, int revents) {
  /* nop */
}

static void prepare_cb (EV_P_ ev_prepare *w, int revents) {
  struct econtext *ctx = (struct econtext *)(((char *)w) - offsetof (struct econtext, pw));
  gint timeout;
  int i;

  g_main_context_dispatch (ctx->gc);

  g_main_context_prepare (ctx->gc, &ctx->maxpri);

  while (ctx->afd < (ctx->nfd = g_main_context_query  (
                      ctx->gc,
                      ctx->maxpri,
                      &timeout,
                      ctx->pfd,
                      ctx->afd))) {
    free (ctx->pfd);
    free (ctx->iow);

    ctx->afd = 1;
    while (ctx->afd < ctx->nfd)
      ctx->afd <<= 1;

    ctx->pfd = (GPollFD*)malloc (ctx->afd * sizeof (GPollFD));
    ctx->iow = (ev_io*)malloc (ctx->afd * sizeof (ev_io));
  }

  for (i = 0; i < ctx->nfd; ++i) {
      GPollFD *pfd = ctx->pfd + i;
      ev_io *iow = ctx->iow + i;

      pfd->revents = 0;

      ev_io_init (
        iow,
        io_cb,
        pfd->fd,
        (pfd->events & G_IO_IN ? EV_READ : 0)
         | (pfd->events & G_IO_OUT ? EV_WRITE : 0)
      );
      iow->data = (void *)pfd;
      ev_set_priority (iow, EV_MINPRI);
      ev_io_start (EV_A iow);
    }

  if (timeout >= 0)
    {
      ev_timer_set (&ctx->tw, timeout * 1e-3, 0.);
      ev_timer_start (EV_A &ctx->tw);
    }
}

static void check_cb (EV_P_ ev_check *w, int revents) {
  struct econtext *ctx = (struct econtext *)(((char *)w) - offsetof (struct econtext, cw));
  int i;

  for (i = 0; i < ctx->nfd; ++i)
    {
      ev_io *iow = ctx->iow + i;

      if (ev_is_pending (iow))
        {
          GPollFD *pfd = ctx->pfd + i;
          int revents = ev_clear_pending (EV_A iow);

          pfd->revents |= pfd->events &
            ((revents & EV_READ ? G_IO_IN : 0)
             | (revents & EV_WRITE ? G_IO_OUT : 0));
        }

      ev_io_stop (EV_A iow);
    }

  if (ev_is_active (&ctx->tw))
    ev_timer_stop (EV_A &ctx->tw);

  g_main_context_check (ctx->gc, ctx->maxpri, ctx->pfd, ctx->nfd);
}

static struct econtext default_context;

extern "C" void init(Handle<Object> target) {
  HandleScope scope;

  GMainContext *gc     = g_main_context_default();

  struct econtext *ctx = &default_context;

  ctx->gc  = g_main_context_ref(gc);
  ctx->nfd = 0;
  ctx->afd = 0;
  ctx->iow = 0;
  ctx->pfd = 0;

  ev_prepare_init (&ctx->pw, prepare_cb);
  ev_set_priority (&ctx->pw, EV_MINPRI);
  ev_prepare_start (EV_DEFAULT_ &ctx->pw);

  ev_check_init (&ctx->cw, check_cb);
  ev_set_priority (&ctx->cw, EV_MAXPRI);
  ev_check_start (EV_DEFAULT_ &ctx->cw);

  ev_init (&ctx->tw, timer_cb);
  ev_set_priority (&ctx->tw, EV_MINPRI);

  gtk_init(NULL, NULL);

  // Position constants.
  NGTK_DEFINE_CONSTANT(target, "WIN_POS_NONE",             GTK_WIN_POS_NONE);
  NGTK_DEFINE_CONSTANT(target, "WIN_POS_CENTER",           GTK_WIN_POS_CENTER);
  NGTK_DEFINE_CONSTANT(target, "WIN_POS_MOUSE",            GTK_WIN_POS_MOUSE);
  NGTK_DEFINE_CONSTANT(target, "WIN_POS_CENTER_ALWAYS",    GTK_WIN_POS_CENTER_ALWAYS);
  NGTK_DEFINE_CONSTANT(target, "WIN_POS_CENTER_ON_PARENT", GTK_WIN_POS_CENTER_ON_PARENT);

  // MessageDialog constants
  NGTK_DEFINE_CONSTANT(target, "DIALOG_MODAL",               GTK_DIALOG_MODAL);
  NGTK_DEFINE_CONSTANT(target, "DIALOG_DESTROY_WITH_PARENT", GTK_DIALOG_DESTROY_WITH_PARENT);
  NGTK_DEFINE_CONSTANT(target, "DIALOG_NO_SEPARATOR",        GTK_DIALOG_NO_SEPARATOR);

  // Message types.
  NGTK_DEFINE_CONSTANT(target, "MESSAGE_INFO",     GTK_MESSAGE_INFO);
  NGTK_DEFINE_CONSTANT(target, "MESSAGE_WARNING",  GTK_MESSAGE_WARNING);
  NGTK_DEFINE_CONSTANT(target, "MESSAGE_QUESTION", GTK_MESSAGE_QUESTION);
  NGTK_DEFINE_CONSTANT(target, "MESSAGE_ERROR",    GTK_MESSAGE_ERROR);
  NGTK_DEFINE_CONSTANT(target, "MESSAGE_OTHER",    GTK_MESSAGE_OTHER);

  // Buttons.
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_NONE",      GTK_BUTTONS_NONE);
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_OK",        GTK_BUTTONS_OK);
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_CLOSE",     GTK_BUTTONS_CLOSE);
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_CANCEL",    GTK_BUTTONS_CANCEL);
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_YES_NO",    GTK_BUTTONS_YES_NO);
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_OK_CANCEL", GTK_BUTTONS_OK_CANCEL);

  // Response constants
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_NONE",         GTK_RESPONSE_NONE);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_REJECT",       GTK_RESPONSE_REJECT);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_ACCEPT",       GTK_RESPONSE_ACCEPT);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_DELETE_EVENT", GTK_RESPONSE_DELETE_EVENT);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_OK",           GTK_RESPONSE_OK);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_CANCEL",       GTK_RESPONSE_CANCEL);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_CLOSE",        GTK_RESPONSE_CLOSE);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_YES",          GTK_RESPONSE_YES);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_NO",           GTK_RESPONSE_NO);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_APPLY",        GTK_RESPONSE_APPLY);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_HELP",         GTK_RESPONSE_HELP);

  Window::Initialize(target);
  MessageDialog::Initialize(target);
  Button::Initialize(target);

  Loop::Initialize(target);
}

} // namespace ngtk

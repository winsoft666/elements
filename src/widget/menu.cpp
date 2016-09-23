/*=================================================================================================
   Copyright (c) 2016 Joel de Guzman

   Licensed under a Creative Commons Attribution-ShareAlike 4.0 International.
   http://creativecommons.org/licenses/by-sa/4.0/
=================================================================================================*/
#include <photon/widget/menu.hpp>

namespace photon
{
   void basic_popup_button::layout(context const& ctx)
   {
      basic_button::layout(ctx);

      auto pu_limits = _popup->limits(ctx);
      rect  bounds = {
            ctx.bounds.left + 3, ctx.bounds.bottom,
            ctx.bounds.left + 3 + pu_limits.min.x, full_extent
         };

      context new_ctx{ ctx.view, ctx.canvas, _popup.get(), bounds };
      _popup->bounds(bounds);
      _popup->layout(new_ctx);
   }

   widget* basic_popup_button::click(context const& ctx, mouse_button btn)
   {
      if (btn.down)
      {
         if (state(true))
         {
            auto on_click = [this](context const& ctx, mouse_button btn)
            {
               _popup->close(ctx);
               this->value(0);
               ctx.view.refresh();
            };

            _popup->open(ctx, on_click);
            ctx.view.refresh();
         }
      }
      else
      {
         if (!value() || !hit_test(ctx, btn.pos))
         {
            // simulate a menu click:
            btn.down = true;
            rect  bounds = _popup->bounds();
            context new_ctx{ ctx.view, ctx.canvas, _popup.get(), bounds };
            _popup->click(new_ctx, btn);
         }
      }
      return this;
   }

   void basic_popup_button::drag(context const& ctx, mouse_button btn)
   {
      ctx.view.refresh();
   }

   bool basic_popup_button::key(context const& ctx, key_info k)
   {
      if (k.key == key_code::escape)
      {
         _popup->close(ctx);
         state(false);
         ctx.view.refresh();
         return true;
      }
      return false;
   }

   bool basic_popup_button::focus(focus_request r)
   {
      return true;
   }

   void basic_menu_item_widget::draw(context const& ctx)
   {
      if (ctx.bounds.includes(ctx.view.cursor_pos()))
      {
         auto& canvas_ = ctx.canvas;

         canvas_.begin_path();
         canvas_.round_rect(ctx.bounds, 2);
         canvas_.fill_style(get_theme().indicator_color.opacity(0.6));
         canvas_.fill();
      }
      proxy_base::draw(ctx);
   }

   widget* basic_menu_item_widget::hit_test(context const& ctx, point p)
   {
      if (ctx.bounds.includes(p))
         return this;
      ctx.view.refresh(ctx);
      return 0;
   }

   widget* basic_menu_item_widget::click(context const& ctx, mouse_button btn)
   {
      if (/*btn.down && */on_click)
         on_click();
      return proxy_base::click(ctx, btn);
   }

   bool basic_menu_item_widget::cursor(context const& ctx, point p, cursor_tracking status)
   {
      bool hit = ctx.bounds.includes(p);
      if (status == cursor_tracking::leaving || hit)
         ctx.view.refresh();
      proxy_base::cursor(ctx, p, status);
      return hit;
   }

   bool basic_menu_item_widget::is_control() const
   {
      return true;
   }
}

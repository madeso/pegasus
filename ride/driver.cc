#include "driver.h"

#include "ride/str.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <functional>


namespace ride
{
    vec2 operator+(const vec2& lhs, const vec2& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }


    Rect Rect::CreateWestFromMaxSize(int max_size) const
    {
        return {position, {std::min(max_size, size.x), size.y}};
    }


    Font::~Font()
    {
    }


    Driver::~Driver()
    {
    }


    Painter::~Painter()
    {
    }


    App::~App()
    {
    }
    


    struct Document
    {
        std::vector<std::string> lines =
        {
            "abcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHIJabcdefghijABCDEFGHI",
            "         111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999",
            "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789",
            ""
        };
        
        int GetNumberOfLines() const
        {
            return lines.size();
        }

        std::string GetLineAt(int y) const
        {
            if(y < 0) { return ""; }
            else if(y >= lines.size()) { return ""; }
            else { return lines[y]; }
        }

        Document()
        {
            LoadFile(__FILE__);
        }

        void LoadFile(const std::string& path)
        {
            lines.clear();

            std::ifstream file;
            file.open(path.c_str(), std::ios::in | std::ios::binary);
            if(file.is_open() == false)
            {
                return;
            }

            std::string line;
            while(getline(file,line))
            {
                lines.emplace_back(line);
            }
        }
    };


    struct RectScope
    {
        Painter* painter;

        RectScope(Painter* p, const Rect& rect) : painter(p)
        {
            painter->PushClip(rect);
        }

        ~RectScope()
        {
            painter->PopClip();
        }
    };


    struct Settings
    {
        vec2 scroll_spacing = {3, 3};

        bool render_linenumber = true;
        int left_gutter_padding = 3;
        int right_gutter_padding = 6;
        int editor_padding_left = 6;

        int statusbar_padding_bottom = 3;
        int statusbar_padding_top = 3;
        int statusbar_padding_right = 6;
    };


    struct View
    {
        Rect rect;
        std::shared_ptr<Driver> driver;
        std::shared_ptr<Font> font;
        std::shared_ptr<Document> document;
        std::shared_ptr<Settings> settings;

        int requested_cursor_x = 0;
        vec2 cursor = {0, 0};
        vec2 scroll = {0, 0};

        View
        (
            Rect r,
            std::shared_ptr<Driver> d,
            std::shared_ptr<Font> f,
            std::shared_ptr<Document> doc,
            std::shared_ptr<Settings> s
        ) : rect(r), driver(d), font(f), document(doc), settings(s) {}

        void StepRight(int x)
        {
            const int next_x = std::max(0, cursor.x + x);
            if(document && cursor.y < document->GetNumberOfLines())
            {
                cursor.x = std::min<int>(document->GetLineAt(cursor.y).length(), next_x);
            }
            requested_cursor_x = cursor.x;

            FocusCursor();
        }

        int GetLineNumberSize()
        {
            if(settings == nullptr) { return 0; }
            if(document == nullptr) { return 0; }
            if(driver == nullptr) { return 0; }

            if(settings->render_linenumber == false) { return 0; }
            
            return driver->GetSizeOfString(font, Str{} << document->GetNumberOfLines() + 1).width;
        }

        void StepDown(int y)
        {
            if(document)
            {
                cursor.y = std::min<int>(std::max(0, cursor.y + y), document->GetNumberOfLines());
                cursor.x = std::min<int>(document->GetLineAt(cursor.y).length(), requested_cursor_x);
            }

            FocusCursor();
        }

        void InsertStringAtCursor(const std::string& str)
        {
            if(document == nullptr) { return; }
            if(cursor.x < 0) { return; }
            if(cursor.y < 0) { return; }

            if(cursor.y == document->GetNumberOfLines())
            {
                document->lines.emplace_back(str);
                cursor.x = str.length();
            }
            else
            {
                auto line = document->GetLineAt(cursor.y);
                if(line.length() < cursor.x )
                {
                    // add spaces
                    const auto number_of_spaces = cursor.x - line.length();
                    line = line + std::string(number_of_spaces, ' ');
                }
                line.insert(cursor.x, str);
                document->lines[cursor.y] = line;
                cursor.x += str.length();
            }
        }

        void LimitScroll()
        {
            if(document == nullptr) { return; }
            if(settings == nullptr) { return; }

            const auto limy = [this](int lines) { if(scroll.y > lines) { scroll.y = lines; } };

            // limy(document->GetNumberOfLines() + 1);
            limy(document->GetNumberOfLines() + 1 - GetWindowHeightInLines());

            // todo(Gustav): also limit based on width

            if(scroll.x < 0) { scroll.x = 0; }
            if(scroll.y < 0) { scroll.y = 0; }
        }

        void ScrollDown(int y)
        {
            scroll.y += y;
            LimitScroll();
        }

        void ScrollRight(int x)
        {
            scroll.x += x;
            LimitScroll();
        }

        int GetWindowHeightInLines() const
        {
            if(font == nullptr) { return 0; }
            if(font->line_height <= 0) { return 0; }

            const auto lines = rect.size.y / static_cast<float>(font->line_height);
            return static_cast<int>(std::floor(lines));
        }

        int GetWindowWidthInChars()
        {
            if(font == nullptr) { return 0; }
            if(font->char_width <= 0) { return 0; }

            const auto gutter_width = settings->left_gutter_padding + settings->right_gutter_padding + GetLineNumberSize() + settings->editor_padding_left;
            const auto window_width = std::max(0, rect.size.x - gutter_width);

            const auto chars = window_width / static_cast<float>(font->char_width);
            const auto ci = static_cast<int>(std::floor(chars));

            return ci;
        }

        void FocusCursorHeight()
        {
            if(settings == nullptr) { return; }

            const auto window_height = GetWindowHeightInLines();

            const auto cursor_up = cursor.y - settings->scroll_spacing.y;
            const auto cursor_down = cursor.y + settings->scroll_spacing.y;

            if(scroll.y > cursor_up)
            {
                scroll.y = cursor_up;
            }

            {
                const auto steps = scroll.y + window_height - cursor_down -1;
                if(steps < 0)
                {
                    scroll.y -= steps;
                }
            }
        }

        void FocusCursorWidth()
        {
            if(settings == nullptr) { return; }

            const auto window_width = GetWindowWidthInChars();

            const auto cursor_left = cursor.x - settings->scroll_spacing.x;
            const auto cursor_right = cursor.x + settings->scroll_spacing.x;

            if(scroll.x > cursor_left)
            {
                scroll.x = cursor_left;
            }

            {
                const auto steps = scroll.x + window_width - cursor_right;
                if(steps < 0)
                {
                    scroll.x -= steps;
                }
            }
        }

        void FocusCursor()
        {
            FocusCursorHeight();
            FocusCursorWidth();
            LimitScroll();
        }

        void Draw
        (
            Painter* painter
        )
        {
            const auto foreground_color = Rgb{0,0,0};
            const auto background_color = Rgb{180, 180, 180};
            const auto gutter_color = Rgb{160, 160, 160};
            const auto linenumber_color = foreground_color;

            if(settings == nullptr) { return; }
            if(driver == nullptr) { return; }

            painter->Rect(rect, background_color, std::nullopt);

            if(font && document)
            {
                const auto scope = RectScope{painter, rect};

                const bool render_linenumber = settings->render_linenumber;
                const int left_gutter_padding = settings->left_gutter_padding;
                const int right_gutter_padding = settings->right_gutter_padding;
                const int editor_padding_left = settings->editor_padding_left;

                const auto line_number_size = GetLineNumberSize();

                const auto gutter_rect = rect.CreateWestFromMaxSize(left_gutter_padding + line_number_size + right_gutter_padding);
                painter->Rect(gutter_rect, gutter_color, std::nullopt);
                
                const auto lower_right = rect.position + rect.size;
                
                auto draw = rect.position;
                auto current_scroll = scroll;

                for(; draw.y < lower_right.y; draw.y += font->line_height)
                {
                    if(current_scroll.y >= 0 && current_scroll.y < document->GetNumberOfLines()+1)
                    {
                        const auto substr = [](const std::string& str, int offset) -> std::string
                        {
                            if(offset >= static_cast<int>(str.length())) { return ""; }
                            else if(offset < 0)
                            {
                                return std::string(-offset, ' ') + str;
                            }
                            else
                            {
                                return str.substr(offset);
                            }
                        };
                        if(render_linenumber && current_scroll.y >= 0)
                        {
                            painter->Text(font, Str{} << current_scroll.y + 1, {draw.x + left_gutter_padding, draw.y}, linenumber_color);
                        }
                        const auto full_line = document->GetLineAt(current_scroll.y);
                        const auto line = substr(full_line, scroll.x);
                        const auto draw_position = vec2{draw.x + gutter_rect.size.x + editor_padding_left, draw.y};

                        if(current_scroll.y == cursor.y)
                        {
                            const auto cursor_x = driver->GetSizeOfString(font, full_line.substr(0, cursor.x - scroll.x)).width;
                            painter->Line
                            (
                                {draw_position.x + cursor_x, draw_position.y},
                                {draw_position.x + cursor_x, draw_position.y + font->line_height},
                                {{0, 0, 0}, 1}
                            );
                        }

                        painter->Text(font, line, draw_position, foreground_color);
                    }

                    current_scroll.y = current_scroll.y + 1;
                }
            }
        }
    };


    struct DocumentInformation
    {
        int line;
        int column;
        int number_of_lines;

        DocumentInformation(int l, int c, int n) : line(l), column(c), number_of_lines(n) {}
    };


    using DocumentInformationProvider = std::function<DocumentInformation ()>;


    struct StatusBar
    {
        std::shared_ptr<Font> font;
        std::shared_ptr<Settings> settings;
        DocumentInformationProvider information_provider;

        StatusBar
        (
            std::shared_ptr<Font> f,
            std::shared_ptr<Settings> s,
            DocumentInformationProvider ip
        ) : font(f), settings(s), information_provider(ip) {}

        void Draw(Painter* painter, const vec2& window_size)
        {
            const Rgb text_color = {0, 0, 0};
            const Rgb bkg = {200, 200, 200};

            const auto info = information_provider();

            const auto draw_height = GetHeight();
            const auto rect = Rect{{0, window_size.y - draw_height}, {window_size.x, draw_height}};

            painter->Rect(rect, bkg, std::nullopt);

            const std::string str = Str{}
                << "Ln " << info.line << " / " << info.number_of_lines << " | "
                << "Col " << info.column
                ;

            const auto text_size = painter->GetSizeOfString(font, str);
            const auto text_pos = vec2
            {
                rect.position.x + rect.size.x - text_size.width - settings->statusbar_padding_right,
                rect.position.y + settings->statusbar_padding_top
            };

            painter->Text(font, str, text_pos, text_color);
        }

        int GetHeight() const
        {
            if(font == nullptr) { return 0; }
            if(settings == nullptr) { return 0; }

            return font->line_height + settings->statusbar_padding_bottom + settings->statusbar_padding_top;
        }
    };


    struct Meta
    {
        bool ctrl;
        bool shift;
        bool alt;

        Meta(bool c, bool s, bool a) : ctrl(c), shift(s), alt(a) {}
    };


    struct Widget
    {
        virtual ~Widget() = default;

        virtual void Draw(Painter* painter) = 0;
        virtual bool OnKey(Key key, const Meta& meta) = 0;
        virtual void OnChar(const std::string& ch) = 0;
    };


    struct DemoWidget : public Widget
    {
        std::shared_ptr<Font> font;
        std::string latest_str = "?";

        Rect rect;

        DemoWidget(std::shared_ptr<Font> f, const Rect& r) : font(f), rect(r) { }
        
        void Draw(Painter* painter)
        {
            const auto background_color = Rgb{180, 180, 180};
            
            painter->Rect(rect, background_color, std::nullopt);
            painter->Text(font, latest_str, rect.position, {0, 0, 0});
            
        }

        bool OnKey(Key key, const Meta& meta)
        {
            return false;
        }

        void OnChar(const std::string& ch)
        {
            latest_str = ch;
        }
    };

    struct TextWidget : public Widget
    {
        View view;

        TextWidget
        (
            std::shared_ptr<Driver> driver,
            std::shared_ptr<Font> font,
            std::shared_ptr<Document> document,
            std::shared_ptr<Settings> settings
        )
            : view({{10, 20}, {400, 420}}, driver, font, document, settings)
        {
        }

        DocumentInformation GetCurrentDocumentInformation() const
        {
            return
            {
                view.cursor.y + 1,
                view.cursor.x + 1,
                view.document->GetNumberOfLines()
            };
        }
        
        void Draw(Painter* painter) override
        {
            view.Draw(painter);
        }

        bool OnKey(Key key, const Meta& meta) override
        {
            const auto ctrl = meta.ctrl;

            bool handled = true;

            switch(key)
            {
            case Key::Left:
                if(ctrl) { view.ScrollRight(-1); }
                else { view.StepRight(-1); }
                break;
            case Key::Right:
                if(ctrl) { view.ScrollRight(1); }
                else { view.StepRight(1); }
                break;
            case Key::Up:
                if(ctrl) { view.ScrollDown(-1); }
                else { view.StepDown(-1); }
                break;
            case Key::Down:
                if(ctrl) { view.ScrollDown(1); }
                else { view.StepDown(1); }
                break;
            default:
                handled = false;
                break;
            }

            return handled;
        }

        void OnChar(const std::string& ch) override
        {
            view.InsertStringAtCursor(ch);
        }
    };


    struct RideApp : App
    {
        std::shared_ptr<Driver> driver;

        std::shared_ptr<Font> font_ui;
        std::shared_ptr<Font> font_code;
        std::shared_ptr<Font> font_big;

        std::shared_ptr<Document> document;
        std::shared_ptr<Settings> settings;

        TextWidget widget;
        StatusBar statusbar;
        DemoWidget demo_widget;

        Widget* active_widget;

        vec2 window_size = vec2{0,0};

        RideApp(std::shared_ptr<Driver> d)
            : driver(d)
            , font_ui(d->CreateUiFont(12))
            , font_code(d->CreateCodeFont(8))
            , font_big(d->CreateUiFont(100))
            , document(std::make_shared<Document>())
            , settings(std::make_shared<Settings>())
            , widget(driver, font_code, document, settings)
            , statusbar
                (
                    font_code,
                    settings,
                    [this]()
                    {
                        return widget.GetCurrentDocumentInformation();
                    }
                )
            , demo_widget(font_big, {{50, 600}, {300, 300}})
            , active_widget(&widget)
        {
        }

        void OnSize(const vec2& new_size) override
        {
            window_size = new_size;

            driver->Refresh();
        }

        void OnPaint(Painter* painter) override
        {
            painter->Rect({{0, 0}, window_size}, Rgb{230, 230, 230}, std::nullopt);

            painter->Text(font_ui, "File | Code | Help", {40, 00}, {0, 0, 0});

            widget.Draw(painter);
            demo_widget.Draw(painter);
            statusbar.Draw(painter, window_size);
        }

        void OnMouseMoved(const vec2& new_position) override
        {
            // driver->Refresh();
        }

        void OnMouseLeftWindow() override
        {
            driver->Refresh();
        }

        void OnMouseButton(MouseState state, MouseButton button) override
        {
            driver->Refresh();
        }

        void OnMouseScroll(float scroll, int lines) override
        {
        }

        bool ctrl = false;
        bool alt = false;
        bool shift = false;
        bool OnKey(bool down, Key key) override
        {
            if(key == Key::Control) { ctrl = down; driver->Refresh(); return true; }
            if(key == Key::Alt) { alt = down; driver->Refresh(); return true; }
            if(key == Key::Shift) { shift = down; driver->Refresh(); return true; }

            if(down)
            {
                if(key == Key::Tab)
                {
                    // tab switches between the widgets
                    active_widget = active_widget==&demo_widget ? static_cast<Widget*>(&widget) : static_cast<Widget*>(&demo_widget);
                    driver->Refresh();
                }
                else
                {
                    const bool handled = active_widget->OnKey(key, {ctrl, shift, alt});

                    if(handled)
                    {
                        driver->Refresh();
                    }
                }
            }

            if(ctrl || alt)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        void OnChar(const std::string& ch) override
        {
            active_widget->OnChar(ch);
            driver->Refresh();
        }
    };

    std::shared_ptr<App>
    CreateApp(std::shared_ptr<Driver> driver)
    {
        return std::make_shared<RideApp>(driver);
    }
}

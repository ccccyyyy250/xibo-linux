#pragma once

#include "Widget.hpp"

#include <gtkmm/overlay.h>

class MainLayoutView : public Widget
{
public:
    struct WidgetInfo
    {
        int width;
        int height;
        int x;
        int y;
    };

    using WidgetsWithInfo = std::map<std::shared_ptr<Widget>, WidgetInfo>;

    MainLayoutView(int width, int height);

    void addRegion(const std::shared_ptr<Widget>& child, int x, int y, int z);
    void removeRegion(const std::shared_ptr<Widget>& child);
    void removeRegions();
    void addBackground(const std::shared_ptr<Widget>& mainChild);
    void removeBackground();
    bool hasBackground() const;

    void showAll() override;
    void setSize(int width, int height) override;
    void scale(double scaleX, double scaleY) override;

    Gtk::Overlay& get() override;

private:
    bool onGetRegionPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc);
    WidgetsWithInfo::iterator findRegion(Gtk::Widget* widget);
    void scaleRegions(double scaleX, double scaleY);
    void updateOffsets();

private:
    Gtk::Overlay m_overlayLayout;
    std::shared_ptr<Widget> m_mainChild;
    WidgetsWithInfo m_regions;
    int m_xOffset = 0;
    int m_yOffset = 0;

};
/*
    SPDX-FileCopyrightText: 2015 Martin Gräßlin <mgraesslin@kde.org>
    SPDX-FileCopyrightText: 2020 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#ifndef WAYLAND_SERVER_PLASMA_SHELL_INTERFACE_H
#define WAYLAND_SERVER_PLASMA_SHELL_INTERFACE_H

#include <QObject>

#include <KWaylandServer/kwaylandserver_export.h>

class QSize;
struct wl_resource;

namespace KWaylandServer
{

class Display;
class SurfaceInterface;
class PlasmaShellSurfaceInterface;

class PlasmaShellInterfacePrivate;
class PlasmaShellSurfaceInterfacePrivate;

/**
 * @brief Global for the org_kde_plasma_shell interface.
 *
 * The PlasmaShellInterface allows to add additional information to a SurfaceInterface.
 * It goes beyond what a ShellSurfaceInterface provides and is adjusted toward the needs
 * of the Plasma desktop.
 *
 * A server providing this interface should think about how to restrict access to it as
 * it allows to perform absolute window positioning.
 *
 * @since 5.4
 **/
class KWAYLANDSERVER_EXPORT PlasmaShellInterface : public QObject
{
    Q_OBJECT

public:
    explicit PlasmaShellInterface(Display *display, QObject *parent);
    virtual ~PlasmaShellInterface();

Q_SIGNALS:
    /**
     * Emitted whenever a PlasmaShellSurfaceInterface got created.
     **/
    void surfaceCreated(KWaylandServer::PlasmaShellSurfaceInterface*);

private:
    QScopedPointer<PlasmaShellInterfacePrivate> d;
};

/**
 * @brief Resource for the org_kde_plasma_shell_surface interface.
 *
 * PlasmaShellSurfaceInterface gets created by PlasmaShellInterface.
 *
 * @since 5.4
 **/
class KWAYLANDSERVER_EXPORT PlasmaShellSurfaceInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~PlasmaShellSurfaceInterface();

    /**
     * @returns the SurfaceInterface this PlasmaShellSurfaceInterface got created for
     **/
    SurfaceInterface *surface() const;
    /**
     * @returns the requested position in global coordinates.
     **/
    QPoint position() const;
    /**
     * @returns Whether a global position has been requested.
     **/
    bool isPositionSet() const;

    /**
     * Describes possible roles this PlasmaShellSurfaceInterface can have.
     * The role can be used by the server to e.g. change the stacking order accordingly.
     **/
    enum class Role {
        Normal, ///< A normal surface
        Desktop, ///< The surface represents a desktop, normally stacked below all other surfaces
        Panel, ///< The surface represents a panel (dock), normally stacked above normal surfaces
        OnScreenDisplay, ///< The surface represents an on screen display, like a volume changed notification
        Notification, ///< The surface represents a notification @since 5.24
        ToolTip, ///< The surface represents a tooltip @since 5.24
        CriticalNotification, ///< The surface represents a critical notification, like battery is running out @since 5.58
    };
    /**
     * @returns The requested role, default value is @c Role::Normal.
     **/
    Role role() const;
    /**
     * Describes how a PlasmaShellSurfaceInterface with role @c Role::Panel should behave.
     **/
    enum class PanelBehavior {
        AlwaysVisible, ///< The panel should be always visible
        AutoHide, ///< The panel auto hides at a screen edge and returns on mouse press against edge
        WindowsCanCover, ///< Windows are allowed to go above the panel, it raises on mouse press against screen edge
        WindowsGoBelow ///< Window are allowed to go below the panel
    };
    /**
     * @returns The PanelBehavior for a PlasmaShellSurfaceInterface with role @c Role::Panel
     * @see role
     **/
    PanelBehavior panelBehavior() const;

    /**
     * @returns true if this window doesn't want to be listed
     * in the taskbar
     * @since 5.5
     **/
    bool skipTaskbar() const;

    /**
     * @returns true if this window doesn't want to be listed
     * in a window switcher
     * @since 5.47
     **/
    bool skipSwitcher() const;

    /**
     * Informs the PlasmaShellSurfaceInterface that the auto-hiding panel got hidden.
     * Once it is shown again the method {@link showAutoHidingPanel} should be used.
     *
     * @see showAutoHidingPanel
     * @see panelAutoHideHideRequested
     * @see panelAutoHideShowRequested
     * @since 5.28
     **/
    void hideAutoHidingPanel();

    /**
     * Informs the PlasmaShellSurfaceInterface that the auto-hiding panel got shown again.
     *
     * @see hideAutoHidingPanel
     * @see panelAutoHideHideRequested
     * @see panelAutoHideShowRequested
     * @see 5.28
     **/
    void showAutoHidingPanel();

    /**
     * Whether a PlasmaShellSurfaceInterface wants to have focus.
     *
     * By default some PlasmaShell roles do not get focus, but the PlasmaShellSurfaceInterface can
     * request that it wants to have focus. The compositor can use this information to
     * pass focus to the surface.
     * @since 5.28
     **/
    //TODO KF6 rename to something generic
    bool panelTakesFocus() const;

    /**
     * @returns The PlasmaShellSurfaceInterface for the @p native resource.
     * @since 5.5
     **/
    static PlasmaShellSurfaceInterface *get(wl_resource *native);

    bool visible();

    enum class WindowType {
        TYPE_WALLPAPER = 2000,
        TYPE_DESKTOP = 2001,
        TYPE_DIALOG = 2002,
        TYPE_SYS_SPLASH = 2003,
        TYPE_SEARCH_BAR = 2004,
        TYPE_NOTIFICATION = 2005,
        TYPE_CRITICAL_NOTIFICATION = 2006,
        TYPE_INPUT_METHOD = 2007,
        TYPE_INPUT_METHOD_DIALOG = 2008,
        TYPE_DND = 2009,
        TYPE_DOCK = 2010,
        TYPE_STATUS_BAR = 2011,
        TYPE_STATUS_BAR_PANEL = 2012,
        TYPE_TOAST = 2013,
        TYPE_KEYGUARD = 2014,
        TYPE_PHONE = 2015,
        TYPE_SYSTEM_DIALOG = 2016,
        TYPE_SYSTEM_ERROR = 2017,
        TYPE_VOICE_INTERACTION = 2018,
        TYPE_SYSTEM_OVERLAY = 2019,
	TYPE_SCREENSHOT = 2020,
        TYPE_BOOT_PROGRESS = 2021,
        TYPE_POINTER = 2022,
        TYPE_LAST_SYS_LAYER = 2099,
        TYPE_BASE_APPLICATION = 1,
        TYPE_APPLICATION = 2,
        TYPE_APPLICATION_STARTING = 3,
        TYPE_APPLICATION_OVERLAY = 4,
        TYPE_LAST_APPLICATION_WINDOW = 99,
        TYPE_UNKNOW = -1
    };

    WindowType windowType() const;
Q_SIGNALS:
    /**
     * A change of global position has been requested.
     **/
    void positionChanged();
    /**
     * A change of the role has been requested.
     **/
    void roleChanged();
    /**
     * A change of the panel behavior has been requested.
     **/
    void panelBehaviorChanged();
    /**
     * A change in the skip taskbar property has been requested
     */
    void skipTaskbarChanged();
    /**
     * A change in the skip switcher property has been requested
     **/
    void skipSwitcherChanged();

    void visibleChanged();
    /**
     * A surface with Role Panel and PanelBehavior AutoHide requested to be hidden.
     *
     * The compositor should inform the PlasmaShellSurfaceInterface about the actual change.
     * Once the surface is hidden it should invoke {@link hideAutoHidingPanel}. If the compositor
     * cannot hide the surface (e.g. because it doesn't border a screen edge) it should inform
     * the surface through invoking {@link showAutoHidingPanel}. This method should also be invoked
     * whenever the surface gets shown again due to triggering the screen edge.
     *
     * @see hideAutoHidingPanel
     * @see showAutoHidingPanel
     * @see panelAutoHideShowRequested
     * @since 5.28
     **/
    void panelAutoHideHideRequested();

    /**
     * A surface with Role Panel and PanelBehavior AutoHide requested to be shown.
     *
     * The compositor should inform the PlasmaShellSurfaceInterface about the actual change.
     * Once the surface is shown it should invoke {@link showAutoHidingPanel}.
     *
     * @see hideAutoHidingPanel
     * @see showAutoHidingPanel
     * @see panelAutoHideHideRequested
     * @since 5.28
     **/
    void panelAutoHideShowRequested();

    /*
     * Emitted when panelTakesFocus changes
     * @see panelTakesFocus
     * @since 5.66
     */
    void panelTakesFocusChanged();

    void windowTypeChanged();
private:
    friend class PlasmaShellInterfacePrivate;
    explicit PlasmaShellSurfaceInterface(SurfaceInterface *surface, wl_resource *resource);
    QScopedPointer<PlasmaShellSurfaceInterfacePrivate> d;
};

}

Q_DECLARE_METATYPE(KWaylandServer::PlasmaShellSurfaceInterface::Role)
Q_DECLARE_METATYPE(KWaylandServer::PlasmaShellSurfaceInterface::PanelBehavior)

#endif

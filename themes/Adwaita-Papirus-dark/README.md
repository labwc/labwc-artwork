# Adwaita Papirus Dark

This theme tries to mimic the appearance of the [Adwaita Dark GTK](https://gnome.pages.gitlab.gnome.org/libadwaita/) theme and [colors](https://gnome.pages.gitlab.gnome.org/libadwaita/doc/1.2/named-colors.html) together with the Pairus Dark icon theme.

Recommended `rc.xml` theme settings:

```xml
  <theme>
    <name>Adwaita-Papirus-Dark</name>
    <titlebar layout=":menu,iconify,max,close" showTitle="yes" />
    <cornerRadius>14</cornerRadius>
    <dropShadows>yes</dropShadows>
    <font place="ActiveWindow" name="Cantarell" size="12" weight="bold" />
    <font place="InactiveWindow" name="Cantarell" size="12" weight="bold" />
    <font place="MenuHeader" name="Cantarell" size="12" weight="bold" />
    <font place="MenuItem" name="Cantarell" size="12" />
    <font place="OnScreenDisplay" name="Cantarell" size="12" />
  </theme>
```

Recommended Gnome Tweaks settings:

- Appearance
  - Icons: Papirus-Dark
  - Legacy Applications (GTK3): Adw-gtk3-dark
- Titlebar Buttons
  - Maximize: On
  - Minimize: On
  - Placement: Right

The icons are based on the icons found in the [Adwaita-dark button theme](https://github.com/labwc/labwc-artwork/tree/main/button-themes/svg/Adwaita-dark) in this repository and on the icons of the great [Papirus icon theme](https://github.com/PapirusDevelopmentTeam/papirus-icon-theme).

This theme requires `labwc` version 0.8.1 or newer.

Obligatory screenshot:

![Screenshot](https://github.com/user-attachments/assets/8752fbb5-7ff7-4973-8f55-94cdbf6e331d)

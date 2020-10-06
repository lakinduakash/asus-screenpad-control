all:
	mkdir -p ui/build
	cd ui && $(MAKE)

install: ui/build/screenpad-control
	install -Dm755 ui/build/screenpad-control /usr/bin/screenpad-control
	install -Dm644 desktop/asus-screenpad-control.png /usr/share/icons/hicolor/512x512/apps/asus-screenpad-control.png
	install -Dm644 desktop/asus-screenpad-control.desktop /usr/share/applications/asus-screenpad-control.desktop
	install -Dm755 asus-screenpad-permission-update /etc/init.d/asus-screenpad-permission-update
	gtk-update-icon-cache -f /usr/share/icons/hicolor/

uninstall:
	rm -f /usr/bin/screenpad-control
	rm -f /usr/share/icons/hicolor/512x512/apps/asus-screenpad-control.png
	rm -f /usr/share/applications/asus-screenpad-control.desktop
	rm -f /etc/init.d/asus-screenpad-permission-update

.PHONY: clean

clean:
	cd ui && $(MAKE) clean
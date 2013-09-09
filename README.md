alarm-fake-hwclock
==================

**Written by Xan Manning** inspired by `fake-hwclock` by Steve McIntyre

Save/restore system clock on machines without working RTC hardware.

Dead simple C program initially designed for Arch Linux ARM that works like fake-hwclock on Debian.

It is recommended to install this via the ArchLinux AUR, see https://aur.archlinux.org/packages/fake-hwclock-git/ for a package in the ArchLinux AUR.


Installation (from AUR)
-----------------------

Requires `CMake` to build.

Please follow the instructions from https://wiki.archlinux.org/index.php/Arch_User_Repository#Installing_packages



Installation (not using AUR)
----------------------------

Requires `CMake` to build. Suggested packages: `ntp` and `cron`.

	CMake -DCMAKE_INSTALL_PREFIX=/usr .
	make
	make install


To set system time on start up and save system time on halt, enable the systemd service:

	systemctl enable fake-hwclock.service


To keep `fake-hwclock` up to date in case of power failure, install and enable `ntp` and add the following job to your root `crontab` (`sudo crontab -e`):

	*/15 * * * * /usr/bin/fake-hwclock



License
-------

	/* -----------------------------------------------------------------------------
	 * "THE (COFFEE)WARE LICENSE" (Rev. 1):
	 * 
	 * <xan.manning@gmail.com> 
	 * wrote this file. As long as you retain this notice you can do whatever you
	 * want with this stuff. If we meet some day, and you think this stuff is
	 * worth it, you can buy me the above drink(s) in return.
	 *
	 * Xan Manning
	 * ----------------------------------------------------------------------------- 
	 */

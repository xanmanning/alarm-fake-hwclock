alarm-fake-hwclock
==================

**Written by Xan Manning** inspired by `fake-hwclock` by Steve McIntyre

Save/restore system clock on machines without working RTC hardware.

Dead simple C program initially designed for Arch Linux ARM that works like fake-hwclock on Debian.

It is recommended to install this via the ArchLinux AUR, see https://aur.archlinux.org/packages/fake-hwclock-git/ for a package in the ArchLinux AUR.


Installation (from AUR)
-----------------------

Please follow the instructions from https://wiki.archlinux.org/index.php/Arch_User_Repository#Installing_packages



Installation (not using AUR)
----------------------------

Suggested packages: `ntp` and `cron` or enable `systemd-timesyncd`

	make
	make install


To set system time on start up and save system time on halt as well as run every 15 min,  enable the systemd service:

	systemctl enable fake-hwclock

This will also activate a systemd timer unit that will run every 15 min.



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

===========
perp helper
===========

:Author: Joe Hura <dev@creatingtrouble.com>
:Site: `Creating Trouble`_

.. _Creating Trouble: http://creatingtrouble.com


Introduction
------------

A quick and easy way to generate new services to be monitored by `perp`_.
Set It, Forget It.

Uses templates to quickly generate required rc.main and rc.log files
for a particular service.

Symbolic link management assists in managing which services are currently
active and available for monitoring.

.. _perp: http://b0llix.net/perp/


Requirements
------------

* perp to be installed
* bash interpreter


Information
-----------

* depending on your perp install, you may need to run perphelper as sudo


Install
-------

Copy files from contrib/perphelper to a desired location
  or
Use the location of the perp directory

Make sure bin/perpheler is executable and that perphelper/bin is in your $PATH


Configuration
-------------

Config file found in config/perphelper.conf

There are only a couple of variables to adjust.

PERP_LOC - where perp was installed, this is typically /etc/perp

PERPHELPER_BASE_PATH - where you want the perphelper managed services to live



Commands
--------

* perphelper

  Brings up usage and help

* perphelper conf

  Shows configuration paths for perphelper

* perphelper list-available

  Lists available services that were created with 'make'

* perphelper list-active

  Lists all active services being monitored by 'perp'

  Active services are a subset of those that are available

* perphelper create <service_name>

  Create a new perp service template named service_name

  This creates required rc.main and rc.log files for your service from
  templates in the templates directory.

* perphelper register <service_name>

  Registers and activates service_name links with perp
  This effectively links an available service and executes:
    perpctl A service_name

* perphelper unregister <service_name>

  Unregisters and deactivates service_name with perp

  This effectively removes the links for active services and executes:
    perpctl X service_name


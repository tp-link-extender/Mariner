#include "pch.h"
#include "Util.h"

#include <iostream>
#include <string>

const std::vector<std::string> Util::AllowedHosts {
	"xtcy.dev",
	"www.xtcy.dev",
	"clientsettings.api.xtcy.dev",

	"roblox.com",
	"files.heliodex.cf",
	"heliodex.cf"
};

const char Util::Domain[] = "xtcy.dev";
const char Util::Name[] = "Mercury";
const char Util::ClientSettings[] = "clientsettingsapi.xtcy.dev";
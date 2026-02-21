#include "pch.h"
#include "Util.h"

#include <iostream>
#include <string>

const std::vector<std::string> Util::AllowedHosts {
	"mercs.dev",
	"www.mercs.dev",
	"api.mercs.dev",
	"setup.mercs.dev",
	"mercury.test:5173",
	"localhost",

	"roblox.com",
	"www.roblox.com",
	"files.heliodex.cf",
	"heliodex.cf"
};

const char Util::Domain[] = "mercs.dev";
const char Util::Name[] = "Mercury";
const char Util::ClientSettings[] = "clientsettingsapi.mercs.dev";
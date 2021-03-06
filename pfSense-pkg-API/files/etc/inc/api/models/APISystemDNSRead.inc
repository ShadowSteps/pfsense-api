<?php
//   Copyright 2021 Jared Hendrickson
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

require_once("api/framework/APIModel.inc");
require_once("api/framework/APIResponse.inc");


class APISystemDNSRead extends APIModel {
    # Create our method constructor
    public function __construct() {
        parent::__construct();
        $this->privileges = ["page-all", "page-system"];
    }

    public function action() {
        $this->validate_payload();
        return APIResponse\get(0, $this->validated_data);
    }

    public function validate_payload() {
        $this->validated_data["dnsserver"] = [];
        $this->validated_data["dnsallowoverride"] = false;
        $this->validated_data["dnslocalhost"] = false;
        if (!empty($this->config["system"]["dnsserver"])) {
            $this->validated_data["dnsserver"] = $this->config["system"]["dnsserver"];
        }
        if (array_key_exists("dnsallowoverride", $this->config["system"])) {
            $this->validated_data["dnsallowoverride"] = true;
        }

        # Validate this field as needed for pfSense 2.4.x
        # TODO: remove this conditional once pfSense 2.4 nears EOL
        if (APITools\get_pfsense_version()["program"] < 250) {
            if (array_key_exists("dnslocalhost", $this->config["system"])) {
                $this->validated_data["dnslocalhost"] = true;
            }
        }
        else {
            if ($this->config["system"]["dnslocalhost"] === "remote") {
                $this->validated_data["dnslocalhost"] = false;
            } else {
                $this->validated_data["dnslocalhost"] = true;
            }
        }

    }
}

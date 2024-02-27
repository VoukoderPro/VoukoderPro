#pragma once

#include "nlohmann/json.hpp"

namespace VoukoderPro
{
    static nlohmann::json voukoderpro_schema = R"(
{
  "$id": "https://voukoder.org/schemas/voukoderpro",
  "$schema": "http://json-schema.org/draft-07/schema",
  "type": "object",
  "patternProperties": {
    "^.{1,256}$": {
      "required": [
        "name",
        "nodes"
      ],
      "properties": {
        "name": {
          "type": "string",
          "minLength": 0,
          "maxLength": 256
        },
        "nodes": {
          "type": "array",
          "items": {
            "$ref": "#/definitions/node"
          }
        }
      }
    }
  },
  "additionalProperties": false,
  "definitions": {
    "identifier": {
      "type": "string",
      "pattern": "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"
    },
    "node": {
      "type": "object",
      "required": [
        "id",
        "type",
        "mediaType",
        "pos",
        "data",
        "inputs",
        "outputs"
      ],
      "properties": {
        "id": {
          "$ref": "#/definitions/identifier"
        },
        "type": {
          "type": "string",
          "enum": [
            "input",
            "filter",
            "encoder",
            "muxer",
            "output",
            "postproc"
          ]
        },
        "mediaType": {
          "type": "string",
          "enum": [
            "video",
            "audio",
            "mux",
            "out"
          ]
        },
        "pos": {
          "type": "object",
          "required": [
            "x",
            "y"
          ],
          "properties": {
            "x": {
              "type": "number"
            },
            "y": {
              "type": "number"
            }
          }
        },
        "data": {
          "type": "object"
        },
        "inputs": {
          "type": "array",
          "description": "",
          "items": {
            "type": "array",
            "items": {
              "$ref": "#/definitions/identifier"
            }
          }
        },
        "outputs": {
          "type": "array",
          "description": "",
          "items": {
            "type": "array",
            "items": {
              "$ref": "#/definitions/identifier"
            }
          }
        }
      }
    }
  }
}
	)"_json;
}
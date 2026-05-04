"""MCP server para Unreal Engine 5 via Remote Control API (puerto 30010)."""
import json
import sys
import urllib.request
import urllib.error

UE_URL = "http://127.0.0.1:30010"


def ue_post(endpoint: str, payload: dict) -> dict:
    data = json.dumps(payload).encode()
    req = urllib.request.Request(
        f"{UE_URL}{endpoint}",
        data=data,
        headers={"Content-Type": "application/json"},
        method="POST",
    )
    try:
        with urllib.request.urlopen(req, timeout=10) as resp:
            return json.loads(resp.read())
    except urllib.error.URLError as e:
        return {"error": str(e)}


def ue_get(endpoint: str) -> dict:
    try:
        with urllib.request.urlopen(f"{UE_URL}{endpoint}", timeout=10) as resp:
            return json.loads(resp.read())
    except urllib.error.URLError as e:
        return {"error": str(e)}


TOOLS = [
    {
        "name": "ue_get_actors",
        "description": "Lista todos los actores en el nivel actual de UE5",
        "inputSchema": {"type": "object", "properties": {}, "required": []},
    },
    {
        "name": "ue_spawn_actor",
        "description": "Spawna un actor Blueprint en el nivel de UE5",
        "inputSchema": {
            "type": "object",
            "properties": {
                "blueprint_path": {"type": "string", "description": "Ruta del Blueprint, ej: /Game/Blueprints/BP_Player"},
                "location_x": {"type": "number"},
                "location_y": {"type": "number"},
                "location_z": {"type": "number"},
            },
            "required": ["blueprint_path"],
        },
    },
    {
        "name": "ue_call_function",
        "description": "Llama una función en un objeto/actor de UE5",
        "inputSchema": {
            "type": "object",
            "properties": {
                "object_path": {"type": "string", "description": "Ruta del objeto UE5"},
                "function_name": {"type": "string"},
                "parameters": {"type": "object"},
            },
            "required": ["object_path", "function_name"],
        },
    },
    {
        "name": "ue_get_property",
        "description": "Lee el valor de una propiedad de un actor/objeto de UE5",
        "inputSchema": {
            "type": "object",
            "properties": {
                "object_path": {"type": "string"},
                "property_name": {"type": "string"},
            },
            "required": ["object_path", "property_name"],
        },
    },
    {
        "name": "ue_set_property",
        "description": "Establece el valor de una propiedad en un actor/objeto de UE5",
        "inputSchema": {
            "type": "object",
            "properties": {
                "object_path": {"type": "string"},
                "property_name": {"type": "string"},
                "property_value": {},
            },
            "required": ["object_path", "property_name", "property_value"],
        },
    },
    {
        "name": "ue_open_level",
        "description": "Abre un nivel/mapa en el editor de UE5",
        "inputSchema": {
            "type": "object",
            "properties": {
                "level_path": {"type": "string", "description": "Ruta del nivel, ej: /Game/Maps/MainLevel"},
            },
            "required": ["level_path"],
        },
    },
    {
        "name": "ue_save_level",
        "description": "Guarda el nivel actual en UE5",
        "inputSchema": {"type": "object", "properties": {}, "required": []},
    },
    {
        "name": "ue_editor_status",
        "description": "Comprueba si el editor de UE5 está activo y responde",
        "inputSchema": {"type": "object", "properties": {}, "required": []},
    },
]


def handle_tool(name: str, args: dict) -> str:
    if name == "ue_editor_status":
        result = ue_get("/remote/info")
        if "error" in result:
            return f"Editor NO disponible: {result['error']}"
        return f"Editor UE5 activo: {json.dumps(result, indent=2)}"

    elif name == "ue_get_actors":
        result = ue_post("/remote/search/actors", {"Query": "", "Limit": 100})
        return json.dumps(result, indent=2)

    elif name == "ue_spawn_actor":
        payload = {
            "ObjectPath": args["blueprint_path"],
            "Location": {
                "X": args.get("location_x", 0),
                "Y": args.get("location_y", 0),
                "Z": args.get("location_z", 0),
            },
        }
        result = ue_post("/remote/object/spawn", payload)
        return json.dumps(result, indent=2)

    elif name == "ue_call_function":
        payload = {
            "objectPath": args["object_path"],
            "functionName": args["function_name"],
            "parameters": args.get("parameters", {}),
            "generateTransaction": True,
        }
        result = ue_post("/remote/object/call", payload)
        return json.dumps(result, indent=2)

    elif name == "ue_get_property":
        payload = {
            "objectPath": args["object_path"],
            "access": "READ_ACCESS",
            "propertyName": args["property_name"],
        }
        result = ue_post("/remote/object/property", payload)
        return json.dumps(result, indent=2)

    elif name == "ue_set_property":
        payload = {
            "objectPath": args["object_path"],
            "access": "WRITE_ACCESS",
            "propertyName": args["property_name"],
            "propertyValue": {"value": args["property_value"]},
            "generateTransaction": True,
        }
        result = ue_post("/remote/object/property", payload)
        return json.dumps(result, indent=2)

    elif name == "ue_open_level":
        payload = {
            "objectPath": "/Script/UnrealEd.Default__EditorLevelLibrary",
            "functionName": "LoadLevel",
            "parameters": {"LevelName": args["level_path"]},
            "generateTransaction": False,
        }
        result = ue_post("/remote/object/call", payload)
        return json.dumps(result, indent=2)

    elif name == "ue_save_level":
        payload = {
            "objectPath": "/Script/UnrealEd.Default__EditorLevelLibrary",
            "functionName": "SaveCurrentLevel",
            "parameters": {},
            "generateTransaction": False,
        }
        result = ue_post("/remote/object/call", payload)
        return json.dumps(result, indent=2)

    return f"Herramienta desconocida: {name}"


def send(msg: dict):
    sys.stdout.write(json.dumps(msg) + "\n")
    sys.stdout.flush()


def main():
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        try:
            msg = json.loads(line)
        except json.JSONDecodeError:
            continue

        msg_id = msg.get("id")
        method = msg.get("method", "")

        if method == "initialize":
            send({
                "jsonrpc": "2.0", "id": msg_id,
                "result": {
                    "protocolVersion": "2024-11-05",
                    "capabilities": {"tools": {}},
                    "serverInfo": {"name": "ue5-mcp", "version": "1.0.0"},
                },
            })

        elif method == "notifications/initialized":
            pass

        elif method == "tools/list":
            send({"jsonrpc": "2.0", "id": msg_id, "result": {"tools": TOOLS}})

        elif method == "tools/call":
            tool_name = msg.get("params", {}).get("name", "")
            tool_args = msg.get("params", {}).get("arguments", {})
            result_text = handle_tool(tool_name, tool_args)
            send({
                "jsonrpc": "2.0", "id": msg_id,
                "result": {"content": [{"type": "text", "text": result_text}]},
            })

        elif msg_id is not None:
            send({"jsonrpc": "2.0", "id": msg_id, "error": {"code": -32601, "message": f"Method not found: {method}"}})


if __name__ == "__main__":
    main()

function export_scene(){
// In a controller or one-time initializer
var file = file_text_open_write("room_data.h");

// Write header guard
file_text_write_string(file, "#ifndef ROOM_DATA_H\n#define ROOM_DATA_H\n\n");

// Array of object instance data
file_text_write_string(file, "typedef struct {\n    const char* name;\n    float x;\n    float y;\n} ObjectInstance;\n\n");

file_text_write_string(file, "static const ObjectInstance room_instances[] = {\n");

// Loop through all instances and write C structs
with (all) {
    if (id != noone && !instance_is_destroyed(id)) {
        var obj_name = object_get_name(object_index);
        var line = "    {\"" + obj_name + "\", " + string(x) + ", " + string(y) + "},\n";
        file_text_write_string(file, line);
    }
}

file_text_write_string(file, "};\n\n#define ROOM_INSTANCE_COUNT ");
file_text_write_string(file, string(instance_number(all)));
file_text_write_string(file, "\n\n#endif // ROOM_DATA_H\n");

file_text_close(file);
}
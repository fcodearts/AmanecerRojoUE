import unreal

bp = unreal.load_asset("/Game/Blueprints/BP_IsometricCamera")
sub = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
root = sub.k2_gather_subobject_data_for_blueprint(bp)[0]

# Añadir SpringArm
p = unreal.AddNewSubobjectParams(parent_handle=root, new_class=unreal.SpringArmComponent, blueprint_context=bp)
arm_h, err = sub.add_new_subobject(p)
print("SpringArm err:", err)

sub.rename_subobject(arm_h, unreal.Text("BrazoCamera"))

arm_data = sub.k2_find_subobject_data_from_handle(arm_h)
arm_obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(arm_data)
print("SpringArm obj:", arm_obj)

if arm_obj:
    arm_obj.set_editor_property("target_arm_length", 1200.0)
    arm_obj.set_editor_property("relative_rotation", unreal.Rotator(-45.0, 45.0, 0.0))
    arm_obj.set_editor_property("use_pawn_control_rotation", False)
    arm_obj.set_editor_property("inherit_pitch", False)
    arm_obj.set_editor_property("inherit_yaw", False)
    arm_obj.set_editor_property("inherit_roll", False)
    arm_obj.set_editor_property("enable_camera_lag", True)
    arm_obj.set_editor_property("camera_lag_speed", 5.0)
    print("SpringArm propiedades OK")

    p2 = unreal.AddNewSubobjectParams(parent_handle=arm_h, new_class=unreal.CameraComponent, blueprint_context=bp)
    cam_h, err2 = sub.add_new_subobject(p2)
    print("Camera err:", err2)
    sub.rename_subobject(cam_h, unreal.Text("CamaraIsometrica"))

unreal.EditorAssetLibrary.save_asset("/Game/Blueprints/BP_IsometricCamera")
print("DONE: BP_IsometricCamera configurado correctamente")

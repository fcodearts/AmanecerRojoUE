import unreal
sub = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
metodos = [m for m in dir(sub) if not m.startswith("_")]
print("SubobjectDataSubsystem metodos:", metodos)
handle_test = unreal.SubobjectDataHandle()
print("SubobjectDataHandle metodos:", [m for m in dir(handle_test) if not m.startswith("_")])

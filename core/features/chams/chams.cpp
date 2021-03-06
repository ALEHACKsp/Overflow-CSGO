#include "../../../dependencies/common_includes.hpp"
#include "../features.hpp"


std::vector<std::string> mat_names_model = {
	"debug/debugambientcube", // Normal Material            
	"metallic_chams",                       // Metallic Material
	"cherry_flat",                                  // Red Material
};

std::vector<std::string> mat_names_other = {
	"models/inventory_items/trophy_majors/crystal_clear",               // Crystal Clear Material
	"models/inventory_items/trophy_majors/crystal_blue",                // Crystal Blue Matrial
	"models/gibs/glass/glass",
	"models/inventory_items/trophy_majors/gold",
	"models/inventory_items/cologne_prediction/cologne_prediction_glass",
	"models/inventory_items/wildfire_gold/wildfire_gold_detail",
	"models/inventory_items/trophy_majors/velvet",
	"models/inventory_items/music_kit/darude_01/mp3_detail",
	"models/extras/speech_info",
	"models/weapons/customization/stickers/cologne2014/esl_c",
	"models/inventory_items/contributor_map_tokens/contributor_charset_color",
	"models/inventory_items/dreamhack_trophies/dreamhack_star_blur",
	"models/player/t_guerilla/t_guerilla",
	"models/inventory_items/trophy_majors/silver_winners",
	"models/props_shacks/fishing_net01",
	"models/player/ct_fbi/ct_fbi_glass",                                                    // Platinum Material
	"models/inventory_items/trophy_majors/gloss",                           // Gloss Material
	"vgui/achievements/glow",                                                                   // Glow Material
	"models/inventory_items/dogtags/dogtags_outline",                   // Dogtags Outline Material
	"models/inventory_items/dogtags/dogtags_lightray",              // Dogtags Lightray Material
	"models/inventory_items/dogtags/dogtags",                               // Dogtags Material
	"models/inventory_items/hydra_crystal/hydra_crystal",               // Hydra Crystal Material
	"models/inventory_items/hydra_crystal/hydra_crystal_detail", // Hydra Crystal Detail Material

};

void custom_materials() 
{
	std::ofstream("csgo/materials/overflow_textured.vmt") << R"#("VertexLitGeneric" {
            "$basetexture" "vgui/white_additive"
            "$ignorez"      "0"
            "$envmap"       ""
            "$nofog"        "1"
            "$model"        "1"
            "$nocull"       "0"
            "$selfillum"    "1"
            "$halflambert"  "1"
            "$znearer"      "0"
            "$flat"         "0"
			"$phong"		"1"
			"$rimlight"		"1"
        })#";

	std::ofstream("csgo\\materials\\sensum_reflective.vmt") << R"#("VertexLitGeneric"
{
      "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1"
      "$reflectivity" "1"
	  "$phong" "1"
	  "$rimlight" "1"
	"$envmap"    "env_cubemap"
	"$envmaptint" "[.3 .3 .3]" //change this for diff colors etc (that's the metallic part btw)
	"$envmapcontrast" "1"
	"$envmapsaturation" "1.0"
	"$phong" "1"
	"$phongexponent" "15.0" //change this for the amount of material covered in shine
	"$normalmapalphaenvmask" "1"
	"$phongboost"                "6.0" //change this for the shine brightness
}
)#";

	std::ofstream("csgo\\materials\\sensum_flat.vmt") << R"#("UnlitGeneric"
	{
	  "$basetexture" "vgui/white"
	  "$ignorez"      "0"
	  "$envmap"       ""
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$selfillum"    "1"
	  "$halflambert"  "1"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";

	std::ofstream("csgo\\materials\\sensum_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
}

void features::visuals::chams_run(hooks::dme::fn original, void* thisptr, void* context, void* state, const model_render_info_t& info, matrix3x4_t* custom_bone_to_world)
{
	if (!menu.config.chams)
		return original(thisptr, context, state, info, custom_bone_to_world);


	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return original(thisptr, context, state, info, custom_bone_to_world);

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return original(thisptr, context, state, info, custom_bone_to_world);

	if (!info.model)
		return original(thisptr, context, state, info, custom_bone_to_world);

	// get our entity
	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
	if (!entity || !entity->is_alive() || entity->dormant())
		return original(thisptr, context, state, info, custom_bone_to_world);

	static bool create = true;
	if (create)
	{
		custom_materials();
		create = false;
	}

	if (strstr(interfaces::model_info->get_model_name(info.model), "models/player"))
	{
		static int index;
		static i_material* mat = nullptr;
		if (!mat || index != menu.config.chams_type)
			switch (menu.config.chams_type)
			{
			case 0:
				mat = interfaces::material_system->find_material("sensum_regular", TEXTURE_GROUP_MODEL); index = menu.config.chams_type; break;
			case 1:
				mat = interfaces::material_system->find_material("sensum_reflective", TEXTURE_GROUP_MODEL, true, nullptr); index = menu.config.chams_type; break;
			case 2:
				mat = interfaces::material_system->find_material("sensum_flat", TEXTURE_GROUP_MODEL); index = menu.config.chams_type; break;
			case 3:
				mat = interfaces::material_system->find_material("models/gibs/glass/glass", TEXTURE_GROUP_OTHER); index = menu.config.chams_type; break;
			case 4:
				mat = interfaces::material_system->find_material("dev/glow_armsrace.vmt", TEXTURE_GROUP_OTHER); index = menu.config.chams_type; break;
			case 5:
				mat = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER); index = menu.config.chams_type; break;
			case 6:
				mat = interfaces::material_system->find_material("models/inventory_items/wildfire_gold/wildfire_gold_detail", TEXTURE_GROUP_OTHER); index = menu.config.chams_type; break;
			default:
				index = menu.config.chams_type; break;
			}

		mat->increment_reference_count();
		mat->set_material_var_flag(material_var_wireframe, menu.config.wireframe);

		// team
		if (entity->team() == csgo::local_player->team() && menu.config.team_check_chams)
		{
			interfaces::render_view->set_blend(menu.config.f_chams_clr[3]);
			interfaces::render_view->modulate_color(menu.config.f_chams_clr);
			interfaces::model_render->override_material(mat);
		}	

		// enemy
		if (entity->team() != csgo::local_player->team())
		{
			interfaces::render_view->set_blend(menu.config.f_chams_clr[3]);
			interfaces::render_view->modulate_color(menu.config.f_chams_clr);
			interfaces::model_render->override_material(mat);
		}

		// local
		if (menu.config.local_chams && entity == local_player)
		{
			interfaces::render_view->set_blend(menu.config.f_chams_clr[3]);
			interfaces::render_view->modulate_color(menu.config.f_chams_clr);
			interfaces::model_render->override_material(mat);
		}

		// glow
		if (menu.config.chams_type == 4)
		{
			original(thisptr, context, state, info, custom_bone_to_world);
			interfaces::model_render->override_material(mat);
			original(thisptr, context, state, info, custom_bone_to_world);
		}
	}
	original(thisptr, context, state, info, custom_bone_to_world);
	interfaces::model_render->override_material(nullptr);
}


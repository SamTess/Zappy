# Compilation des librairies Raylib et Raygui en .so

RAYLIB_DIR=RaylibCPP
RAYGUI_DIR=RayGUICPP
BUILD_DIR=../build/libs
PLUGINS_DIR=../plugins
INCLUDE=-I$(RAYLIB_DIR) -I$(RAYGUI_DIR) -I$(RAYGUI_DIR)/include -I$(RAYGUI_DIR)/raygui-4.0/src
CXX=g++
CXXFLAGS=-fPIC -shared -O2 $(INCLUDE)

RED=\033[31m
GREEN=\033[32m
YELLOW=\033[33m
BLUE=\033[34m
MAGENTA=\033[35m
CYAN=\033[36m
RESET=\033[0m

all: $(PLUGINS_DIR)/libraylibcpp.so $(PLUGINS_DIR)/libraygui.so
	@mkdir -p ../plugins
	@echo -e "$(GREEN)[OK] Compilation des librairies Raylib et Raygui terminée.$(RESET)"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@echo -e "$(CYAN)[INFO] Création du dossier build/libs.$(RESET)"
$(PLUGINS_DIR):
	@mkdir -p $(PLUGINS_DIR)
	@echo -e "$(CYAN)[INFO] Création du dossier plugins.$(RESET)"

$(BUILD_DIR)/RayLib.o: $(RAYLIB_DIR)/RayLib.cpp | $(BUILD_DIR)
	@echo -e "$(YELLOW)Compilation RayLib.cpp...$(RESET)"
	@$(CXX) -c $(CXXFLAGS) $< -o $@
$(BUILD_DIR)/Window.o: $(RAYLIB_DIR)/window/Window.cpp | $(BUILD_DIR)
	@echo -e "$(YELLOW)Compilation Window.cpp...$(RESET)"
	@$(CXX) -c $(CXXFLAGS) $< -o $@
$(BUILD_DIR)/Shape.o: $(RAYLIB_DIR)/shape/Shape.cpp | $(BUILD_DIR)
	@echo -e "$(YELLOW)Compilation Shape.cpp...$(RESET)"
	@$(CXX) -c $(CXXFLAGS) $< -o $@
$(BUILD_DIR)/Texture.o: $(RAYLIB_DIR)/texture/Texture.cpp | $(BUILD_DIR)
	@echo -e "$(YELLOW)Compilation Texture.cpp...$(RESET)"
	@$(CXX) -c $(CXXFLAGS) $< -o $@
$(BUILD_DIR)/Font.o: $(RAYLIB_DIR)/font/Font.cpp | $(BUILD_DIR)
	@echo -e "$(YELLOW)Compilation Font.cpp...$(RESET)"
	@$(CXX) -c $(CXXFLAGS) $< -o $@
$(BUILD_DIR)/Input.o: $(RAYLIB_DIR)/input/Input.cpp | $(BUILD_DIR)
	@echo -e "$(YELLOW)Compilation Input.cpp...$(RESET)"
	@$(CXX) -c $(CXXFLAGS) $< -o $@
$(BUILD_DIR)/Audio.o: $(RAYLIB_DIR)/audio/Audio.cpp | $(BUILD_DIR)
	@echo -e "$(YELLOW)Compilation Audio.cpp...$(RESET)"
	@$(CXX) -c $(CXXFLAGS) $< -o $@

$(PLUGINS_DIR)/libraylibcpp.so: $(BUILD_DIR)/RayLib.o $(BUILD_DIR)/Window.o $(BUILD_DIR)/Shape.o $(BUILD_DIR)/Texture.o $(BUILD_DIR)/Font.o $(BUILD_DIR)/Input.o $(BUILD_DIR)/Audio.o | $(PLUGINS_DIR)
	@echo -e "$(BLUE)Linkage de libraylibcpp.so...$(RESET)"
	@$(CXX) $(CXXFLAGS) -o $@ $^ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	@echo -e "$(GREEN)[OK] libraylibcpp.so généré dans plugins.$(RESET)"

$(BUILD_DIR)/RayGui.o: $(RAYGUI_DIR)/RayGui.cpp | $(BUILD_DIR)
	@echo -e "$(YELLOW)Compilation RayGui.cpp...$(RESET)"
	@$(CXX) -c $(CXXFLAGS) $< -o $@
$(BUILD_DIR)/raygui_impl.o: $(RAYGUI_DIR)/raygui_impl.cpp | $(BUILD_DIR)
	@echo -e "$(YELLOW)Compilation raygui_impl.cpp...$(RESET)"
	@$(CXX) -c $(CXXFLAGS) $< -o $@

$(PLUGINS_DIR)/libraygui.so: $(BUILD_DIR)/RayGui.o $(BUILD_DIR)/raygui_impl.o | $(PLUGINS_DIR)
	@echo -e "$(BLUE)Linkage de libraygui.so...$(RESET)"
	@$(CXX) $(CXXFLAGS) -o $@ $^ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	@echo -e "$(GREEN)[OK] libraygui.so généré dans plugins.$(RESET)"

clean:
	@rm -f $(BUILD_DIR)/*.o $(PLUGINS_DIR)/libraylibcpp.so $(PLUGINS_DIR)/libraygui.so
	@echo -e "$(MAGENTA)Nettoyage terminé.$(RESET)"

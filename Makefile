# Makefile pour le projet HFT / GBM

# Le compilateur clang (pour Mac)
CXX = clang++
# Options de compilation : C++17, optimisation O2
CXXFLAGS = -std=c++17 -O2 -Iinclude

# Dossiers
SRC_DIR = src
INC_DIR = include

# Liste des fichiers source .cpp
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# Conversion automatique en fichiers objets .o
OBJECTS = $(SOURCES:.cpp=.o)

# Nom de l'exécutable final
EXEC = sim

# La règle par défaut
all: $(EXEC)

# Construction de l'exécutable en liant tous les objets
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation des fichiers source .cpp -> .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Make clean pour nettoyer 
clean:
	rm -f $(SRC_DIR)/*.o $(EXEC)

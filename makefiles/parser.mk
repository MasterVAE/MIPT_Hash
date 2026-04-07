SOURCES_PARSER = parser/main.cpp
				
TARGET_PARSER = $(TARGET_DIR)/parser.out

OBJECTS_PARSER = $(addprefix $(OBJ_DIR)/, $(SOURCES_PARSER:.cpp=.o))

$(TARGET_PARSER): $(OBJECTS_PARSER) | $(TARGET_DIR)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "LINKED PARSER"

parser_b: $(TARGET_PARSER)

parser: $(TARGET_PARSER)
	@./$(TARGET_PARSER)
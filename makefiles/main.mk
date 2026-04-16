SOURCES_MAIN = main/main.cpp main/hash_table.cpp main/list.cpp main/hash_funcs.cpp main/words.cpp
				
TARGET_MAIN = $(TARGET_DIR)/prog.out

OBJECTS_MAIN = $(addprefix $(OBJ_DIR)/, $(SOURCES_MAIN:.cpp=.o))

$(TARGET_MAIN): $(OBJECTS_MAIN) | $(TARGET_DIR) asm
	@$(CC) $(CFLAGS) $^ build/obj/main/nasm.o -o $@
	@echo "LINKED MAIN"

main_b: $(TARGET_MAIN)

main: $(TARGET_MAIN)
	@./$(TARGET_MAIN)

asm:
	@nasm -f elf64 src/main/hash.asm -o build/obj/main/hash.o    
	@echo "COMPILED ASM"  

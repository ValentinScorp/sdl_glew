.PHONY: clean All

All:
	@echo "----------Building project:[ glew_2_1_0 - Debug ]----------"
	@cd "glew_2_1_0" && "$(MAKE)" -f  "glew_2_1_0.mk"
	@echo "----------Building project:[ oasis - Debug ]----------"
	@cd "oasis\proj" && "$(MAKE)" -f  "oasis.mk"
clean:
	@echo "----------Cleaning project:[ glew_2_1_0 - Debug ]----------"
	@cd "glew_2_1_0" && "$(MAKE)" -f  "glew_2_1_0.mk"  clean
	@echo "----------Cleaning project:[ oasis - Debug ]----------"
	@cd "oasis\proj" && "$(MAKE)" -f  "oasis.mk" clean

.PHONY: clean All

All:
	@echo "----------Building project:[ xyzfile - Debug ]----------"
	@cd "xyzfile" && "$(MAKE)" -f  "xyzfile.mk"
clean:
	@echo "----------Cleaning project:[ xyzfile - Debug ]----------"
	@cd "xyzfile" && "$(MAKE)" -f  "xyzfile.mk" clean

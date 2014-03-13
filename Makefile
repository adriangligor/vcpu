# for debug only
#_VARS := $(.VARIABLES)

MODULES := stage1 stage2
MODULE := $(MAKECMDGOALS)

ifneq "$(filter $(MODULE),$(MODULES))" ""
  $(info building module $(MODULE))

  SRC_DIR := $(MODULE)/src
  OUT_DIR := $(MODULE)/out

  # you can use gcc as well
  CC := clang
  CFLAGS := -Wall -Wextra -Wconversion -Werror -pedantic

  # command for calculating dependencies
  DEPCMD := ./depend.sh

  # look for include files in each of the modules
  #CFLAGS += $(patsubst %,-I%,$(MODULES))
  CFLAGS += -I$(SRC_DIR)

  # extra libraries if required
  LIBS := -lm

  # each module will add to this
  SRC :=

  # include the description for each module
  #include $(patsubst %,%/module.mk,$(MODULES))
  include $(MODULE)/module.mk

  # expand sources to include the correct folder
  SRC := $(addprefix $(SRC_DIR)/,$(SRC))

  # determine the object files
  OBJ := $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%.o,$(filter %.c,$(SRC)))

  # link the program
  $(OUT_DIR)/shell : $(OBJ)
	$(CC) $(LIBS) -o $@ $^

  # include the C include dependencies
  include $(OBJ:.o=.d)

  # calculate C include dependencies
  $(OUT_DIR)/%.d: $(SRC_DIR)/%.c
	$(DEPCMD) `dirname $@` $(CFLAGS) $< > $@

  # module target
  .PHONY : $(MODULE)
  $(MODULE) : $(OUT_DIR)/shell
	@echo built module $(MODULE)

else
  .PHONY : all
  all :
	@echo unknown or unspecified module
endif


.PHONY : clean
clean :
	for dir in $(MODULES); do (rm -rfv $$dir/out/*;); done

# for debug only
#$(foreach v,$(filter-out $(_VARS) _VARS,$(.VARIABLES)),$(info $(v) = $($(v))))

BUILD_DIRS = virt0824_temp_driver virt_i2c_controller virt0808_irq_controller user_test 
HOST_MKDIR=mkdir -p
HOST_INSTALL=install

IMAGES_DIR=$(shell pwd)/images
export IMAGES_DIR HOST_MKDIR HOST_INSTALL

all:
	@$(foreach dir, $(BUILD_DIRS),  $(MAKE) -C $(dir) || exit 1;)

install:
	$(if $(wildcard $(IMAGES_DIR)), , $(HOST_MKDIR) $(IMAGES_DIR))
	@$(foreach dir, $(BUILD_DIRS),  $(MAKE) -C $(dir) install || exit 1;)

clean:
	@$(foreach dir, $(BUILD_DIRS),  $(MAKE) -C $(dir) clean || exit 1;)
	rm -rf $(IMAGES_DIR)

.PHONY:all install clean

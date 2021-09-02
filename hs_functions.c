#include <linux/kernel.h>
#include <linux/module.h>

void copy_section(struct module* mod_from, struct module* mod_to, char *section_name);
int quiescence(struct module* mod);

static void copy_section(struct module *mod_from, struct module *mod_to, char *section_name) {
    Elf_Shdr *section_hdrs_from;
    Elf_Shdr *section_hdrs_to;
    Elf_Shdr cpy;
    Elf_Ehdr elf_hdr_from, elf_hdr_to;
    void *target_section_from;
    void *target_section_to;
    int i, j;
    
    if(!(mod_from->klp) || !(mod_to->klp)) {
        return;
    }
    
    elf_hdr_from = mod_from->klp_info->hdr;
    elf_hdr_to = mod_to->klp_info->hdr;
    section_hdrs_from = mod_from->klp_info->sechdrs;
    section_hdrs_to = mod_to->klp_info->sechdrs;
    
    j=0;
    for(i=0; i<elf_hdr_from.e_shnum; i++) {
        if(strncmp(mod_from->klp_info->secstrings + section_hdrs_from[i].sh_name, section_name, strlen(section_name)) == 0) {
            target_section_from = (void *)section_hdrs_from[i].sh_addr;
            cpy = section_hdrs_from[i];
            j=i;
	}
    }
    if(j == 0)
	return;
    for(i=0; i<elf_hdr_to.e_shnum; i++) {
        if(strncmp(mod_to->klp_info->secstrings + section_hdrs_to[i].sh_name, section_name, strlen(section_name)) == 0) {
            target_section_to = (void *)section_hdrs_to[i].sh_addr;
        }
    }
    
    memcpy(target_section_to, target_section_from, cpy.sh_size);
    
    return;
}


int quiescence(struct module* mod) {
    const struct kernel_symbol *sym;
    const char *semaphore_substring;
	unsigned long sem;
	int i;
	
    i = 0;
    sem = 0;
    semaphore_substring = "semaphore";

    while(i < (mod->num_syms)) {
        sym = &(mod->syms[i]);
        if(strstr((const char*)offset_to_ptr(&sym->name_offset), semaphore_substring) != NULL) {
            sem = *(unsigned long*)offset_to_ptr(&sym->value_offset);
            break;
        }
        i++;
    }

    return sem;
}


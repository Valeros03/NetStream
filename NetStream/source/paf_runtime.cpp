#include <kernel.h>
#include <paf.h>
#include <libsysmodule.h>

#include <stdint.h>
// 1. Includiamo l'header della VDSuite che contiene la firma ufficiale per il linker Sony!
#include <vshbridge.h> 


extern "C" {

	// 2. I nostri ID magici
#define SCE_SYSMODULE_INTERNAL_PAF 0x80000016
#define SCE_SYSMODULE_INTERNAL_BXCE 0x80000013
#define SCE_SYSMODULE_INTERNAL_INI_FILE_PROCESSOR 0x80000001
#define SCE_SYSMODULE_INTERNAL_COMMON_GUI_DIALOG 0x80000019

// 3. CREIAMO NOI LA FUNZIONE CHE MANCA AL LINKER!
// NetStream chiamerà questa credendo sia quella di sistema.
	int sceSysmoduleLoadModuleInternalWithArg(uint32_t id, uint32_t arg_size, void *arg, void *unk) {

		if (id == SCE_SYSMODULE_INTERNAL_PAF) {
			int status = 0;
			// Invece di chiedere il permesso al sistema Sysmodule, carichiamo fisicamente 
			// il file del motore grafico PAF dalla memoria della PS Vita!
			// Gli passiamo "arg_size" e "arg", che contengono i famosi 128MB.
			int res = sceKernelLoadStartModule("vs0:sys/external/libScePaf.suprx", arg_size, arg, 0, (const SceKernelLoadModuleOpt*)0, &status);

			if (res >= 0) {
				return 0; // Successo assoluto
			}
			return res; // Errore di caricamento
		}

		return -1;
	}
}
extern "C" {

	typedef struct SceSysmoduleOpt {
		int flags;
		int *result;
		int unused[2];
	} SceSysmoduleOpt;

	int sceAppMgrGrowMemory3(unsigned int a1, int a2);

	void __cxa_set_dso_handle_main(void *dso)
	{

	}

	int _sceLdTlsRegisterModuleInfo()
	{
		return 0;
	}

	int __at_quick_exit()
	{
		return 0;
	}
	
	void user_malloc_init(void)
	{
		int32_t ret = -1, load_res;

		ScePafInit init_param;
		SceSysmoduleOpt sysmodule_opt;

		init_param.global_heap_size = 0x03000000;
		init_param.cdlg_mode = 0;
		init_param.global_heap_alignment = 0;
		init_param.global_heap_disable_assert_on_alloc_failure = 0;

		sysmodule_opt.flags = 0;
		sysmodule_opt.result = &load_res;

		ret = sceSysmoduleLoadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, sizeof(init_param), &init_param, &sysmodule_opt);

		if (ret < 0) {
			sceClibPrintf("[PAF PRX] Loader: 0x%x\n", ret);
			sceClibPrintf("[PAF PRX] Loader result: 0x%x\n", load_res);
		}
	}

	void user_malloc_finalize(void)
	{

	}

	void *user_malloc(size_t size)
	{
		return sce_paf_malloc(size);
	}

	void user_free(void *ptr)
	{
		sce_paf_free(ptr);
	}

	void *user_calloc(size_t nelem, size_t size)
	{
		return sce_paf_calloc(nelem, size);
	}

	void *user_realloc(void *ptr, size_t size)
	{
		return sce_paf_realloc(ptr, size);
	}

	void *user_memalign(size_t boundary, size_t size)
	{
		return sce_paf_memalign(boundary, size);
	}

	void *user_reallocalign(void *ptr, size_t size, size_t boundary)
	{
		sceClibPrintf("[PAF2LIBC] reallocalign is not supported\n");
		abort();
		return NULL;
	}

	int user_malloc_stats(struct malloc_managed_size *mmsize)
	{
		sceClibPrintf("[PAF2LIBC] malloc_stats is not supported\n");
		abort();
		return 0;
	}

	int user_malloc_stats_fast(struct malloc_managed_size *mmsize)
	{
		sceClibPrintf("[PAF2LIBC] malloc_stats_fast is not supported\n");
		abort();
		return 0;
	}

	size_t user_malloc_usable_size(void *ptr)
	{
		return sce_paf_musable_size(ptr);
	}
}

void *user_new(std::size_t size)
{
	void *ret = sce_paf_malloc(size);
	/*
		SceAvPlayer bugfix: yucca::DashManifestParser::FinishParse() tries to use uninitialized pointers in MediaLookUpMmap member
		This bug is not visible with SceLibc because it always memsets heap with 0's and yucca probably checks: if(ptr){...}
	*/
	/*
	if (size == 112)
	{
		sce_paf_memset(ret, 0, size);
	}
	*/
	return ret;
}

void *user_new(std::size_t size, const std::nothrow_t& x)
{
	return sce_paf_malloc(size);
}

void *user_new_array(std::size_t size)
{
	return sce_paf_malloc(size);
}

void *user_new_array(std::size_t size, const std::nothrow_t& x)
{
	return sce_paf_malloc(size);
}

void user_delete(void *ptr)
{
	sce_paf_free(ptr);
}

void user_delete(void *ptr, const std::nothrow_t& x)
{
	sce_paf_free(ptr);
}

void user_delete_array(void *ptr)
{
	sce_paf_free(ptr);
}

void user_delete_array(void *ptr, const std::nothrow_t& x)
{
	sce_paf_free(ptr);
}

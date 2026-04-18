#ifndef _MAIN_H_
#define _MAIN_H_

#include <kernel.h>
#include <paf.h>

using namespace paf;

#ifndef _INTERNAL_FIXES_
#define _INTERNAL_FIXES_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_SYSMODULE_INTERNAL_PAF                  0x80000016
#define SCE_SYSMODULE_INTERNAL_BXCE                 0x80000013
#define SCE_SYSMODULE_INTERNAL_INI_FILE_PROCESSOR   0x80000001
#define SCE_SYSMODULE_INTERNAL_COMMON_GUI_DIALOG    0x80000019
#define SCE_SYSMODULE_INTERNAL_PHOTO_IMPORT_GUI     0x80000010

	int sceSysmoduleLoadModuleInternal(uint32_t id);
	int sceSysmoduleLoadModuleInternalWithArg(uint32_t id, uint32_t arg_size, void *arg, void *unk);

#ifdef __cplusplus
}
#endif
#endif

namespace menu {
	namespace main {
		class NetcheckJob : public job::JobItem
		{
		public:

			enum Type
			{
				Type_Initial,
				Type_NpOnly
			};

			NetcheckJob(Type type) : job::JobItem("NS::NetcheckJob", NULL), m_type(type)
			{

			}

			~NetcheckJob() {}

			int32_t Run();

			void Finish(int32_t result) {}

		private:

			void OnDialogUserCancelEvent(int32_t type);

			Type m_type;
		};

		int32_t OnNpDialogComplete(void *data);
	}
}


#endif

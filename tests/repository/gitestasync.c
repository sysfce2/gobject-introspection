
#include "girepository.h"

static void
test_get_async_function_information_for_interface (void)
{
  GIRepository *repo;
  GITypelib *ret;
  GIBaseInfo *info;
  GError *error;

  repo = g_irepository_get_default ();

  error = NULL;
  ret   = g_irepository_require (repo, "Gio", NULL, 0, &error);
  g_assert (ret != NULL);
  g_assert (error == NULL);

  info = g_irepository_find_by_name (repo, "Gio", "File");
  g_assert (info != NULL);
  g_assert (g_base_info_get_type (info) == GI_INFO_TYPE_INTERFACE);

  GIFunctionInfo *callable_info
      = g_interface_info_find_method (info, "load_contents_async");

  g_assert (g_callable_info_is_async ((GICallableInfo *)callable_info));

  GICallableInfo *sync_info = g_callable_info_get_sync_function (callable_info);
  GICallableInfo *finish_info = g_callable_info_get_finish_function (callable_info);

  g_assert_cmpstr (g_base_info_get_name (sync_info), ==, "load_contents");
  g_assert_cmpstr (g_base_info_get_name (finish_info), ==, "load_contents_finish");

  GICallableInfo *async_info = g_callable_info_get_async_function (sync_info);

  g_assert_cmpstr (g_base_info_get_name (async_info), ==, "load_contents_async");

  g_base_info_unref (async_info);
  g_base_info_unref (sync_info);
  g_base_info_unref (finish_info);
  g_base_info_unref (callable_info);
  g_base_info_unref (info);
}

int
main (int argc, char **argv)
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/girepository/async/"
                   "test_get_async_function_information_for_interface",
                   test_get_async_function_information_for_interface);

  return g_test_run ();
}

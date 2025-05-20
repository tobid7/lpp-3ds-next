#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 ** UnZip *********************************************************************
 *******************************************************************************/

/** @defgroup Zip Zip Library
 *  @{
 */

#include <zlib.h>

/**
 * A zip
 */
typedef void Zip;

/**
 * A file within a zip
 */
typedef struct {
  unsigned char *data; /**<  The file data */
  int size;            /**<  Size of the data */

} ZipFile;

/**
 * Open a Zip file
 *
 * @param filename - Path of the zip to load.
 *
 * @returns A pointer to a ::Zip struct or NULL on error.
 */
Zip *ZipOpen(const char *filename);

/**
 * Close a Zip file
 *
 * @param zip - A valid (previously opened) ::Zip
 *
 * @returns 1 on success, 0 on error
 */
int ZipClose(Zip *zip);

/**
 * Read a file from a zip
 *
 * @param zip - A valid (previously opened) ::Zip
 *
 * @param filename - The file to read within the zip
 *
 * @param password - The password of the file (pass NULL if no password)
 *
 * @returns A ::ZipFile struct containing the file
 */
ZipFile *ZipFileRead(Zip *zip, const char *filename, const char *password);

/**
 * Extract all files from a zip
 *
 * @param zip - A valid (previously opened) ::Zip file
 *
 * @param password - The password of the file (pass NULL if no password)
 *
 * @returns 1 on success, 0 on error.
 */
int ZipExtract(Zip *zip, const char *password);

/**
 * Free the file data previously loaded from a zip
 *
 * @param file - A valid (previously read) ::ZipFile
 */
void ZipFileFree(ZipFile *file);

#ifdef __cplusplus
}
#endif

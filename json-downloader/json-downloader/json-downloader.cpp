// json-downloader.cpp
//

#include "pch.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

int curlMain(int argc, char* argv[]);

void testJson();

int main(int argc, char* argv[])
{
	testJson();
	return 0;
	// return curlMain(argc, argv);
}

void testJson()
{
	// create a JSON object
	json j =
	{
		{"pi", 3.141},
		{"happy", true},
		{"name", "Niels"},
		{"nothing", nullptr},
		{
			"answer", {
				{"everything", 42}
			}
		},
		{"list", {1, 0, 2}},
		{
			"object", {
				{"currency", "USD"},
				{"value", 42.99}
			}
		}
	};

	// add new values
	j["new"]["key"]["value"] = { "another", "list" };

	// count elements
	auto s = j.size();
	j["size"] = s;

	// pretty print with indent of 4 spaces
	std::cout << std::setw(4) << j << '\n';
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

int curlMain(int argc, char* argv[])
{
	CURL *curl_handle;
	static const char *pagefilename = "page.out";
	FILE *pagefile;

	if (argc < 2) {
		printf("Usage: %s <URL>\n", argv[0]);
		return 1;
	}

	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */
	curl_handle = curl_easy_init();

	/* set URL to get here */
	curl_easy_setopt(curl_handle, CURLOPT_URL, argv[1]);

	/* Switch on full protocol/debug output while testing */
	curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

	/* disable progress meter, set to 0L to enable and disable debug output */
	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

	/* open the file */
	pagefile = fopen(pagefilename, "wb");
	if (pagefile) {

		/* write the page body to this file handle */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

		/* get it! */
		curl_easy_perform(curl_handle);

		/* close the header file */
		fclose(pagefile);
	}

	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);

	curl_global_cleanup();

	return 0;
}


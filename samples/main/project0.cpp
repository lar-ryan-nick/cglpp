#include "ModelViewApplication.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "USAGE: " << argv[0] << " <path-to-model-file>" << std::endl;
		return -1;
	}
	ModelViewApplication project0(argv[1]);
	project0.run();
	return 0;
}

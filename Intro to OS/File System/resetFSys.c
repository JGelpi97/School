int main(int argc, char *argv[])
{
	system("rm .directories");
	system("dd bs=1K count=5K if=/dev/zero of=.disk");

}
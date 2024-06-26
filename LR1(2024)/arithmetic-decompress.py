import arithmeticcoding


# Command line main application function.
def main(inputfile, outputfile):
	# Perform file decompression
	with open(outputfile, "wb") as out, open(inputfile, "rb") as inp:
		bitin = arithmeticcoding.BitInputStream(inp)
		freqs = read_frequencies(bitin)
		decompress(freqs, bitin, out)


def read_frequencies(bitin):
	def read_int(n):
		result = 0
		for _ in range(n):
			result = (result << 1) | bitin.read_no_eof()  # Big endian
		return result
	
	freqs = [read_int(32) for _ in range(256)]
	freqs.append(1)  # EOF symbol
	return arithmeticcoding.SimpleFrequencyTable(freqs)


def decompress(freqs, bitin, out):
	dec = arithmeticcoding.ArithmeticDecoder(32, bitin)
	while True:
		symbol = dec.read(freqs)
		if symbol == 256:  # EOF symbol
			break
		out.write(bytes((symbol,)))


# Main launcher
if __name__ == "__main__":
	inputFile = "encode.txt"
	outputFile = "decode.txt"
	main(inputFile, outputFile)

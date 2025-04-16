BYTE_RUN_HASH_BITS = 12
BYTE_RUN_HASH_SIZE = 1 << BYTE_RUN_HASH_BITS
BYTE_RUN_HASH_BITMASK = BYTE_RUN_HASH_SIZE - 1

MAX_RUN_COPY_CHECK_ATTEMPTS = 128

class ByteRunHashTable:
    def __init__(self, buffer_size: int):
        # Buffer size's bits must not toggle any bits in the lower 8 bits of the byte hash, otherwise
        #  it'll throw off the hash.
        assert buffer_size & 0xFF == 0
        self.buffer_size = buffer_size
        self.hash_table = [None] * (buffer_size + BYTE_RUN_HASH_SIZE)
        self.inverse_table = [None] * (buffer_size + BYTE_RUN_HASH_SIZE)
        self.current_hash = 0

    def next_byte_hash(self, existing_hash: int, next_byte: int) -> int:
        return self.buffer_size + (((existing_hash << 4) ^ next_byte) & BYTE_RUN_HASH_BITMASK)

    def insert_byte_hash(self, position: int):
        byte_hash = self.current_hash
        previous_position = self.hash_table[byte_hash]
        if previous_position is not None:
            # We have a value for this hash, and we're about to update what position is in there, so
            #  update the inverse table so we can still find the previous position's entry in the hash
            #  table when we go to clear it.
            self.inverse_table[previous_position] = position
            self.hash_table[position] = previous_position
        self.inverse_table[position] = byte_hash
        self.hash_table[byte_hash] = position

    def clear_entry_at_position(self, position: int):
        hash_table_idx = self.inverse_table[position]
        if hash_table_idx is not None:
            self.hash_table[hash_table_idx] = None
            self.inverse_table[position] = None

    def possible_run_positions(self):
        next_position = self.hash_table[self.current_hash]
        count = 0
        while next_position is not None and count < MAX_RUN_COPY_CHECK_ATTEMPTS:
            yield next_position
            next_position = self.hash_table[next_position]
            count += 1

    def record_byte(self, byte: int):
        self.current_hash = self.next_byte_hash(self.current_hash, byte)
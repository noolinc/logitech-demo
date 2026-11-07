// Aram policy DSL (illustrative, Logi Wheel org): a contracts change must be
// labeled as contract work and carry a schema note when TLV ids change.
package aram

contract_change := input.change.files[_]
contract_change.path == "include/logi/proto.h"

// Copyright © 2025 Jean Silva
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
//                            http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.

use crate::node::{Node, UnmatchedPatternError};

#[cfg(windows)]
pub(crate) const NEWLINE: &str = "\r\n";

#[cfg(not(windows))]
pub(crate) const NEWLINE: &str = "\n";

impl<'a> Node<'a> {
  /// Denotes that a newline node is expected to follow this node.
  pub(crate) fn branch_to_newline<F: Fn(Self) -> Result<Self, UnmatchedPatternError>>(
    self,
    chain: F
  ) -> Result<Self, UnmatchedPatternError> {
    self.branch_to_named("Newline", NEWLINE, chain)
  }
}

#[cfg(test)]
mod tests {
  use crate::node::Node;

  #[test]
  fn succeeds_when_branching_to_a_newline() {
    assert!(Node::default().branch_to_newline(|node| Ok(node)).is_ok())
  }
}

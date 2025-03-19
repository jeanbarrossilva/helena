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

use regex::Regex;

use crate::{
  branch::Branch,
  node::{Node, UnmatchedPatternError}
};

/// Node of a user-defined name for a structure which has either being declared or has already been.
#[derive(Debug)]
pub(crate) struct IdentifierNode<'a, N: Node> {
  /// Textual representation of this node in the AST.
  text: &'a str,

  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32
}

impl<'a, N: Node> IdentifierNode<'a, N> {
  /// Instantiates an identifier node.
  pub(crate) fn new(text: &'a str, column: u32, row: u32) -> Self {
    IdentifierNode {
      text,
      column,
      row
    }
  }
}

impl<'a, N: Node> Node for IdentifierNode<'a, N> {
  fn column(&'a self) -> u32 {
    self.column
  }

  fn row(&'a self) -> u32 {
    self.row
  }

  fn text(&'a self) -> Result<&'a str, UnmatchedPatternError> {
    crate::node::matching(Regex::new(r"[a-zA-Z0-9_]*"), self.text)
  }
}

impl<'a, N1: Node> Branch<'_, _, IdentifierNode<'a, N1>> {
  /// Denotes that an identifier node is expected to follow the current node.
  pub(crate) fn identifier<N2: Node, F: Fn(&IdentifierNode<'a, N1>) -> &Branch<'a, N1, N2>>(
    &mut self,
    text: &'a str,
    chain: F
  ) -> &'a Self {
    self.expect(&IdentifierNode::new(text, self.column_of(text), self.row_of(text)), chain)
  }
}
